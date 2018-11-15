# cpp-server

**Structure (Important files):**
* <code>playbox/</code> - Docker image with compiler and dependencies
* <code>server-source/..</code> - Most important directory contan
  * <code>public/</code> - html, css, js, wasm
  * <code>build/server-source</code> - build folder <strong>Important! Folder must be created every time you clone the project!</strong>
  * <code>main.cpp</code> - server code
  * <code>crow_all.h</code> - Crow, C++ web microframework inspired by Flask 

Extension to C++ microframework Crow

**Build with:**

* [Crow](https://github.com/ipkn/crow)
* C++ libraries: boost/mongocxx/bsoncxx
* MongoDB
* Docker
* CMake


### Important

Note: The following software was tested on various Ubuntu and Debian distributions of Linux, and some of them require to run docker commands as <code>sudo</code>. No guarantees in Win or Mac workability! 

Requirments: cmake, gcc, docker, mlab account

Prerequisites: Knowledge of basic back end development, prior experience with docker is recommended

Best way to get docker is docker's official [site](https://docs.docker.com/install/)

Easiest way to get g++ on Ubuntu or Debian (most of the time) is the following command

```
sudo apt-get install build-essential
```

One way to get cmake is cmake's official [site](https://cmake.org/download/)


#### Before we start

You need to create account at mlabs, and following their official [documentation](https://docs.mlab.com/) create new database. This is the only setup guaranteed to work out of the box.

<strong>The next step is important!</strong> In server-source folder create file keys.js that looks the following way:

```
$ cat keys.h
static const std::string MONGO_URI = "mongodb://<YOUR_DB_LOGIN>:<YOUR_DB_PASSWORD>@ds131777.mlab.com:754993<YOUR_DB_NAME>";
```

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
$ docker run -v /home/aakatev/Documents/git/cpp-server:/usr/src/cpp-server \
-p 8080:8080 -e PORT=8080\
-ti playbox:latest\
/usr/src/cpp-server/server-source/build/server-source
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
$ mkdir /usr/src/cpp-server/server-source/build
cd /usr/src/cpp-server/server-source/build &&\
cmake .. &&\ 
make
```



#### Running development server

After you created container, mounted volume, and successfully compiled server code, you can launch Crow server from <code>playbox</code> container. The server is running at <code>localhost:8080</code>

## Docs v0.0.2

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

