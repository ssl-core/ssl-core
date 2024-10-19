# behavior-zidane


```bash
# to build (from ssl-core root):
docker build --file behavior-ms/behavior-zidane/Dockerfile --target prod --tag "${image_tag}" .

# to run:
docker run -v /tmp/.ssl-core:/tmp/.ssl-core "${image_tag}"
```
