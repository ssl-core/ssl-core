FROM node:20 AS build

ARG SOURCE_DIRECTORY
ARG NGINX_VERSION=1.23.3  

RUN apt update && \
  apt-get install -y build-essential \
  libpcre3        \
  libpcre3-dev    \
  zlib1g          \
  zlib1g-dev      \
  libssl-dev      \
  libgd-dev       \
  libxml2         \
  libxml2-dev     \
  uuid-dev        \
  wget            \
  gnupg

RUN set -x                                                                             && \
  cd /tmp                                                                              && \
  wget -q http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz                      && \
  wget -q http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz.asc                  && \
  tar -xvf nginx-${NGINX_VERSION}.tar.gz

WORKDIR /tmp/nginx-${NGINX_VERSION}

RUN ./configure --with-ld-opt="-static" --with-http_sub_module                    && \
  make install                                                                    && \
  strip /usr/local/nginx/sbin/nginx

RUN ln -sf /dev/stdout /usr/local/nginx/logs/access.log                             && \
  ln -sf /dev/stderr /usr/local/nginx/logs/error.log

WORKDIR /app

COPY ./${SOURCE_DIRECTORY}/package.json .
COPY ./${SOURCE_DIRECTORY}/yarn.lock .
RUN yarn

COPY ./${SOURCE_DIRECTORY} .

RUN yarn build

FROM scratch 

COPY --from=build /etc/passwd /etc/group /etc/
COPY --from=build /usr/local/nginx /usr/local/nginx
COPY --from=build /app/dist /usr/share/nginx/html/
COPY --from=build /app/nginx.conf /usr/local/nginx/conf/

STOPSIGNAL SIGQUIT

EXPOSE 80

ENTRYPOINT ["/usr/local/nginx/sbin/nginx"]

CMD ["-g", "daemon off;"]
