# Tuya IoT Cloud Daemon
This project allows the user to launch the program as a daemon process or just as a simple program. The program is implemented in C using `Tuya IoT Core SDK`. Upon launching the program starts sending memory usage data to Tuya cloud.
## Usage:
* Clone this repository to your local machine.
* Compile the code using: 
```bash
make
```
* Clean up files using: 
```bash
make clean
```
* Install/uninstall the libraries using:
```bash
sudo make install-lib / make uninstall-lib
```
* Run the program:
```bash
./tuya_cloud_daemon -d <device_id> -p <product_id> -s <device_secret> -D
```
