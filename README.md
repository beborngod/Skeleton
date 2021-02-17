# Skeleton 
 - opensource text editor 

## Features
- Multi-Tabs
- Shortcuts
- Themes changing
- Font changing
- PDF writer

### Linux platforms

- Debian based
- RedHat based

Its avaliable too in other Linux platforms but it wasn't tested.

### Debian based installing
- You have to install additional packages

        sudo apt install libxcb-xinerama0

- And next:
        
        chmod +x InstallerSkeleton.run
    
        ./InstallerSkeleton.run

### Debian based installing
    
         chmod +x InstallerSkeleton.run
    
         ./InstallerSkeleton.run


### Deploying from Source (not recomended if you have not Qt5 in your PC):

	git clone https://github.com/beborngod/Skeleton

	cd Skeleton

    cmake --build path/to/build --config Release --target all
  
    make
  
    ./Skeleton