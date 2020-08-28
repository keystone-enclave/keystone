# Building Docker Image

To build the image with master branch:
```bash
docker build -t keystoneenclaveorg/keystone:master
```

dev branch:

```bash
docker build -t keystoneenclaveorg/keystone:dev --build-arg CHECKOUT=dev .
```

any other branches or tags:
```bash
docker build -t keystoneenclaveorg/keystone:<tag> --build-arg CHECKOUT=<tag> .
```
