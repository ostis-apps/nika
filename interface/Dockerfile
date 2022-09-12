#nodejs stage to build frontent 
FROM node:16-alpine AS web-buildenv

WORKDIR /nika-ui 
#Install build-time dependencies
COPY package.json package-lock.json ./
RUN yarn
#Build the interface 
COPY . .
RUN yarn build 

FROM node:16-alpine as runtime 
USER root

WORKDIR /nika-ui
# dependencies
COPY --from=web-buildenv /nika-ui/build /nika-ui/build
COPY --from=web-buildenv /nika-ui/server /nika-ui/server
COPY --from=web-buildenv /nika-ui/package.json /nika-ui/package-lock.json /nika-ui/ 
RUN npm install -g http-server
WORKDIR /nika-ui/build
ENTRYPOINT ["http-server"]
