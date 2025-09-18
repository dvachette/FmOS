![License](https://img.shields.io/badge/license-GNU_GPL_V2-orange)




# FmOS
File Manager OS : A minimalist os created in C and Assembly, aimed to be a didactic project to learn about os development.

## Compile

First you need to cross compile the compiler toolchain, you can do this by running the following command:

```bash
chmod u+x build_toolchain.sh
./build_toolchain.sh
```

Then you can compile the os by running the following command:

```bash
cd src
make
``` 

This will create a bootable iso in `src/isodir` called `fmos.iso`.

## Run

You can run the os using qemu by running the following command:

```bash
cd src
make run
```

## Clean

You can clean the build files by running the following command:

```bash
cd src
make clean
```


## Note
You need to have qemu and grub installed to run the os.

## License
This project is licensed under the GNU General Public License v3.0.

