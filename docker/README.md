# Building Docker Image

To build the image with master branch:
```bash
docker build -t keystoneenclaveorg/keystone:master .
```


dev branch:

```bash
docker build -t keystoneenclaveorg/keystone:dev --build-arg CHECKOUT=dev .
```

any other branches or tags:
```bash
docker build -t keystoneenclaveorg/keystone:<tag> --build-arg CHECKOUT=<tag> .
```

# Building CI images

RV64:
```
docker build -t keystoneenclaveorg/keystone:init-rv64gc --build-arg CHECKOUT=master . --platform linux/x86_64 -f Dockerfile.nobuild
docker push keystoneenclaveorg/keystone:init-rv64gc
```

RV32:

```
docker build -t keystoneenclaveorg/keystone:init-rv32gc --build-arg CHECKOUT=master . --platform linux/x86_64 -f Dockerfile.32.nobuild
docker push keystoneenclaveorg/keystone:init-rv32gc
```
