FROM playbox:latest

WORKDIR /usr/src/cpp-server/server-source/
COPY . .

WORKDIR /usr/src/cpp-server/server-source/build
RUN cmake .
RUN make 
CMD ["./server-source"]