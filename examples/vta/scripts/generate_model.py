# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
"""
Deploy Pretrained Vision Model from MxNet on VTA
================================================
**Author**: `Thierry Moreau <https://homes.cs.washington.edu/~moreau/>`_

This tutorial provides an end-to-end demo, on how to run ImageNet classification
inference onto the VTA accelerator design to perform ImageNet classification tasks.
It showcases Relay as a front end compiler that can perform quantization (VTA
only supports int8/32 inference) as well as graph packing (in order to enable
tensorization in the core) to massage the compute graph for the hardware target.
"""

######################################################################
# Install dependencies
# --------------------
# To use the autotvm package in tvm, we need to install some extra dependencies.
# (change "3" to "2" if you use python2):
#
# .. code-block:: bash
#
#   pip3 install --user mxnet requests "Pillow<7"
#
# Now return to the python code. Import packages.

from __future__ import absolute_import, print_function

import os
import sys, time
from os.path import join

from mxnet.gluon.model_zoo import vision

import tvm
import tvm.micro.testing
from tvm import rpc, autotvm, relay, runtime as tvm_runtime
from tvm.contrib import utils, download

import vta
from vta.top import graph_pack

def main():
    # Check pameters
    if len(sys.argv) != 2:
        print('Usage: generate_model.py <output dir>')
        exit(-1)

    # Make sure that TVM was compiled with RPC=1
    assert tvm.runtime.enabled("rpc")

    ######################################################################
    # Define the platform and model targets
    # -------------------------------------
    # Execute on CPU vs. VTA, and define the model.

    # Load VTA parameters from the 3rdparty/vta-hw/config/vta_config.json file
    env = vta.get_env()

    # Set ``device=arm_cpu`` to run inference on the CPU
    # or ``device=vta`` to run inference on the FPGA.
    device = "vta"
    target = env.target if device == "vta" else env.target_vta_cpu

    # Dictionary lookup for when to start/end bit packing
    pack_dict = {
        "resnet18_v1": ["nn.max_pool2d", "nn.global_avg_pool2d"],
        "resnet34_v1": ["nn.max_pool2d", "nn.global_avg_pool2d"],
        "resnet18_v2": ["nn.max_pool2d", "nn.global_avg_pool2d"],
        "resnet34_v2": ["nn.max_pool2d", "nn.global_avg_pool2d"],
        "resnet50_v2": ["nn.max_pool2d", "nn.global_avg_pool2d"],
        "resnet101_v2": ["nn.max_pool2d", "nn.global_avg_pool2d"],
    }

    # Name of Gluon model to compile
    # The ``start_pack`` and ``stop_pack`` labels indicate where
    # to start and end the graph packing relay pass: in other words
    # where to start and finish offloading to VTA.
    model = "resnet18_v1"
    assert model in pack_dict

    ######################################################################
    # Perform image classification inference
    # --------------------------------------
    # We run classification on an image sample from ImageNet
    # We just need to download the categories files, `synset.txt`
    # and an input test image.

    # Download ImageNet categories
    categ_url = "https://github.com/uwsampl/web-data/raw/main/vta/models/"
    categ_fn = "synset.txt"
    download.download(join(categ_url, categ_fn), categ_fn)
    synset = eval(open(categ_fn).read())

    # # Download test image
    # image_url = "https://homes.cs.washington.edu/~moreau/media/vta/cat.jpg"
    # image_fn = "cat.png"
    # download.download(image_url, image_fn)
    #
    # # Prepare test image for inference
    # image = Image.open(image_fn).resize((224, 224))
    # image = np.array(image) - np.array([123.0, 117.0, 104.0])
    # image /= np.array([58.395, 57.12, 57.375])
    # image = image.transpose((2, 0, 1))
    # image = image[np.newaxis, :]
    # image = np.repeat(image, env.BATCH, axis=0)

    ######################################################################
    # Build the inference graph executor
    # ----------------------------------
    # Grab vision model from Gluon model zoo and compile with Relay.
    # The compilation steps are:
    #
    # 1. Front end translation from MxNet into Relay module.
    # 2. Apply 8-bit quantization: here we skip the first conv layer,
    #    and dense layer which will both be executed in fp32 on the CPU.
    # 3. Perform graph packing to alter the data layout for tensorization.
    # 4. Perform constant folding to reduce number of operators (e.g. eliminate batch norm multiply).
    # 5. Perform relay build to object file.
    # 6. Load the object file onto remote (FPGA device).
    # 7. Generate graph executor, `m`.
    #

    # Load pre-configured AutoTVM schedules
    with autotvm.tophub.context(target):

        # Populate the shape and data type dictionary for ImageNet classifier input
        dtype_dict = {"data": "float32"}
        shape_dict = {"data": (env.BATCH, 3, 224, 224)}

        # Get off the shelf gluon model, and convert to relay
        gluon_model = vision.get_model(model, pretrained=True)

        # Start front end compilation
        mod, params = relay.frontend.from_mxnet(gluon_model, shape_dict)

        # Update shape and type dictionary
        shape_dict.update({k: v.shape for k, v in params.items()})
        dtype_dict.update({k: str(v.dtype) for k, v in params.items()})

        if target.device_name == "vta":
            # Perform quantization in Relay
            # Note: We set opt_level to 3 in order to fold batch norm
            # , skip_conv_layers=[0]
            with tvm.transform.PassContext(opt_level=3):
                with relay.quantize.qconfig(global_scale=8.0):
                    mod = relay.quantize.quantize(mod, params=params)
                # Perform graph packing and constant folding for VTA target
                assert env.BLOCK_IN == env.BLOCK_OUT
                # do device annotation if target is intelfocl or sim
                relay_prog = graph_pack(
                    mod["main"],
                    env.BATCH,
                    env.BLOCK_OUT,
                    env.WGT_WIDTH,
                    start_name=pack_dict[model][0],
                    stop_name=pack_dict[model][1],
                    device_annot=(env.TARGET == "intelfocl"),
                )
        else:
            relay_prog = mod["main"]

        # Compile Relay program with AlterOpLayout disabled
        if target.device_name != "vta":
            with tvm.transform.PassContext(opt_level=3, disabled_pass={"AlterOpLayout"}):
                graph, lib, params = relay.build(
                    relay_prog, target=tvm.target.Target(target, host=env.target_host), params=params
                )
        else:
            if env.TARGET == "intelfocl":
                # multiple targets to run both on cpu and vta
                target = {"cpu": env.target_vta_cpu, "ext_dev": target}
            with vta.build_config(
                    opt_level=3, disabled_pass={"AlterOpLayout", "tir.CommonSubexprElimTIR"}
            ):
                graph, lib, params = relay.build(
                    relay_prog,
                    target=tvm.target.Target(target, host=env.target_host),
                    runtime=tvm.relay.backend.Runtime("crt", {"system-lib": True}), params=params
                )

        os.makedirs(sys.argv[1])
        lib.export_library(sys.argv[1] + '/graphlib.tar')

        with open(sys.argv[1] + '/graph.json', 'w+') as f:
            f.write(graph)

        with open(sys.argv[1] + '/params.bin', 'wb') as f:
            f.write(tvm_runtime.save_param_dict(params))

if __name__ == '__main__':
    main()