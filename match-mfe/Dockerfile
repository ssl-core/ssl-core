FROM node:20-alpine

ARG SOURCE_DIRECTORY

WORKDIR /app

COPY ./${SOURCE_DIRECTORY}/package.json .
COPY ./${SOURCE_DIRECTORY}/yarn.lock .
RUN yarn

COPY ./${SOURCE_DIRECTORY} .

RUN yarn build

EXPOSE 3031

CMD ["yarn", "run", "preview"]
