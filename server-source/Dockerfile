FROM playbox:latest

WORKDIR /usr/src/dmc-stack/server-source/
COPY . .

WORKDIR /usr/src/dmc-stack/server-source/build
RUN cmake ..
RUN make 

EXPOSE 8080
CMD ["./server-source"]
