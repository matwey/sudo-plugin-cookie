# sudo-plugin-cookie

Cookie policy plugin for ```sudo```. This is primarily for build and test environments.

## Building and installing
The project uses ```cmake``` as build system. The building procedure is as follows:
```
mkdir build && cd build
cmake ..
make
make install
```

## Using
1. To enable the plugin put the following into your ```/etc/sudo.conf``` (create the file if it does not exists):
    ```
    Plugin cookie_policy cookie.so
    ```
    You may use absolute path to ```cookie.so``` if the file is not into default place. Cookie file may be specified as the following or default ```/etc/sudo-plugin-cookie``` will be used:
    ```
    Plugin cookie_policy cookie.so cookie_file=/path/to/file
    ```
2. Put text cookie into the cookie file.
3. Run ```sudo``` as follows:
    ```
    sudo SUDO_COOKIE=textcookie ...
    ```

