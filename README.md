# cpp-server

**Structure (Important files):**
* <code>playbox/</code> - Docker image with compiler and dependencies
* <code>server-source/..</code>
  * <code>public/</code> - html, css, js, wasm
  * <code>build/server-source</code> - compiled server code
  * <code>main.cpp</code> - server code
  * <code>crow_all.h</code> - Crow, C++ web microframework inspired by Flask 

Example of a website using C++ server, and WebAssembly modules

**Build with:**

* [Crow](https://github.com/ipkn/crow) on backend
* WebAssembly modules on frontend
* Docker
* CMake
* MongoDB
* C++ libraries: boost/mongocxx/bsoncxx


### Getting Started

- Inside <code>playbox</code> directory, create a new docker container tagged playbox

```
$ docker build -t playbox .
```

#### Docker Cheat Sheet

- Running shell in <code>playbox</code> container

```
$ docker run -ti playbox:latest bash
```

- Note: In general, operations with volumes have the following structure <code>volume_local_path:volume_container_path</code>. In our case, container path is always <code>/usr/src/cpp-server</code>. However, for local path you are going to use your own one.

- Running shell in the container <code>playbox</code> with mounted volume

```
$ docker run -v /home/aakatev/Documents/git/cpp-server:/usr/src/cpp-server -ti playbox:latest bash
``` 

- Running Crow server from <code>playbox</code> container with mounted volume, and port 8080 exposed

```
$ docker run -v /home/aakatev/Documents/git/cpp-server:/usr/src/cpp-server -p 8080:8080 -e PORT=8080 -ti playbox:latest /usr/src/cpp-server/server-source/build/server-source
```

#### C Compiler Cheat Sheet

- Run shell in <code>playbox</code> container, and <code>cd</code> inside <code>/usr/src/cpp-server/server-source/build</code> directory. There, run the following command

```
$ cmake . .
```

Wait for the output, and run

```
$ make
Scanning dependencies of target server-source
[ 50%] Building CXX object CMakeFiles/server-source.dir/main.cpp.o
[100%] Linking CXX executable server-source
[100%] Built target server-source
```

The following output indicates successfull compilation.

- (Shortcut) You can also do it in one line

```
$ cd /usr/src/cpp-server/server-source/build && cmake .. && make
```



#### Running development server

After you created container, mounted volume, and successfully compiled server code, you can launch Crow server from inside of the <code>playbox</code> container. The server is running at <code>localhost:8080</code>

## Docs v2

Docker (from cpp-server folder):
```
docker run -v $PWD:/usr/src/cpp-server -p 8080:8080 -e PORT=8080 -ti playbox:latest bash
```

Heroku (from server-source folder):
```
heroku login
```
```
heroku container:login
```
```
heroku container:push web -a cpp-crow-server
```
```
heroku container:release web -a cpp-crow-server
```

