# perception-brutus


```bash
# to build (from ssl-core root):
docker build --file perception-ms/perception-brutus/Dockerfile --target prod --tag "${image_tag}" .

# to run:
docker run -v /tmp:/tmp "${image_tag}"
```
