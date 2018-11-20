FROM playbox:latest

WORKDIR /usr/src/cpp-server/server-source/
COPY . .

WORKDIR /usr/src/cpp-server/server-source/build
RUN cmake ..
RUN make 

EXPOSE 8080
CMD ["./server-source"]
