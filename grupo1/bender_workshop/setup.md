# Crear workspace
```
$ cd
$ mkdir -p workshop_ws/src
$ cd workshop_ws/src
$ catkin_init_workspace
$ cd ..
$ catkin_make
$ source devel/setup.bash
```

# Crear package
```
$ cd src
$ catkin_create_pkg bender_workshop rospy tf std_msgs std_srvs
```

# Configuración git
```
$ git clone https://github.com/rorromr/bender_test.git
$ cd bender_test
$ git checkout -b grupo<N>
$ mv ~/workshop_ws/src/bender_workshop/ grupo<N>/bender_workshop
$ ln -s ~/bender_test/grupo<N>/bender_workshop ~/workshop_ws/src/bender_workshop
$ git status
$ git add grupo<N>/*
$ git status
$ git commit -m "Primer commit, bender_workshop grupo<N>"
```

# Python package
```
$ roscd bender_workshop
$ mkdir -p src/bender_workshop
$ cd src/bender_workshop
$ touch __init__.py
```

# Python node
```
$ touch test.py
$ chmod +x test.py
```

