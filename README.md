# Skeleton opensource text editor

## Themes
- Transparent
![](images/transparent_theme.png)
- SpyBot
![](images/spybot_theme.png)
- Multi-Tabs
- Font changing
- PDF writer
![](images/creating_pdf.png)

### Platforms
- Fedora Linux
- RHEL
- Debian
- Ubuntu

Its avaliable too in other Linux platforms but it wasn't tested.

## Installing

    chmod +x InstallerSkeleton.run
    
    ./InstallerSkeleton.run


### Deploying from Source (not recomended):

	git clone https://github.com/beborngod/Skeleton

	cd Skeleton

    cmake --build path/to/build --config Release --target all
  
    make
  
    ./Skeleton