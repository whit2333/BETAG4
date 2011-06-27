# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples
export TERM=xterm
# If not running interactively, don't do anything
[ -z "$PS1" ] && return

# don't put duplicate lines in the history. See bash(1) for more options
export HISTCONTROL=ignoredups

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "$debian_chroot" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
xterm-color)
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
    ;;
*)
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
    ;;
esac

# Comment in the above and uncomment this below for a color prompt
#PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PROMPT_COMMAND='echo -ne "\033]0;${USER}@${HOSTNAME}: ${PWD/$HOME/~}\007"'
    ;;
*)
    ;;
esac

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

#if [ -f ~/.bash_aliases ]; then
#    . ~/.bash_aliases
#fi

# enable color support of ls and also add handy aliases
if [ "$TERM" != "dumb" ]; then
    eval "`dircolors -b`"
    alias ls='ls --color=auto'
    #alias dir='ls --color=auto --format=vertical'
    #alias vdir='ls --color=auto --format=long'
fi

# some more ls aliases
#alias ll='ls -l'
#alias la='ls -A'
#alias l='ls -CF'

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
fi



##########################################
# CUE SETUP
##########################################

#
if [ "${HOSTNAME}" == "octavian.jlab.org" ] ; then
  export PATH=/mnt/raid/local/bin:$PATH
  export LD_LIBRARY_PATH=/mnt/raid/local/lib:$LD_LIBRARY_PATH
# VGM 
  export VGM_INSTALL=/mnt/raid/local
  export VGM_SYSTEM=
  export VGM_NO_AGDD=1
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VGM_INSTALL/lib/$VGM_SYSTEM
# InSANE
  export InSANEDIR=/mnt/raid/local/InSANE
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$InSANEDIR/lib
  print "Setting environment for octavian"
fi

#export CLHEP_BASE_DIR=/site/clhep/2.0.4.5/Linux_CentOS5.3-x86_64-gcc4.1.2
#export CLHEP_BASE_DIR=/site/clhep/2.0.4.3/Linux_CentOS5.3-x86_64-gcc4.1.2

#export CLHEP_LIB=CLHEP
#export CLHEP_INCLUDE_DIR=$CLHEP_BASE_DIR/include
#export CLHEP_LIB_DIR=$CLHEP_BASE_DIR/lib

#export GSLINSTALL=$HOME
export G4WORKDIR=/scratch/whit

#export G4INSTALL=/site/geant4/4.9.3.p01/Linux_CentOS5.3-x86_64-gcc4.1.2
#export G4INSTALL=/u/apps/geant4/4.9/geant4.9.3.p01
#export G4INSTALL=/scratch/whit/geant4.9.3.p01
export G4INSTALL=/w/halla-1/g2p/software/geant4.9.4_x86
source $G4INSTALL/env.sh


export G4VIS_BUILD_VRML_DRIVER=
export G4VIS_USE_VRML=

#export G4UI_USE_QT=
#export G4UI_BUILD_QT_SESSION=
#export QTHOME=$QTLIB
export QTLIBS='-L/usr/lib64/qt-3.3/lib -lqt-mt'
export GLQTLIBS='-L/usr/lib64/qt-3.3/lib -lqt-mt' 
#export G4VIS_BUILD_OPENGLQT_DRIVER=
#export G4VIS_USE_OPENGLQT=
#export QTHOME=$QTLIB
#export QTLIBS="-L/u/apps/qt/3.3.8/Linux_RHEL5-i686-gcc4.1.2/lib -lqt-mt"
#export GLQTLIBS="-L/u/apps/qt/3.3.8/Linux_RHEL5-i686-gcc4.1.2/lib -lqt-mt"
#export GLQTLIBS="-L/usr/lib64/qt-3.3/lib -lqt-mt "
#export GLQTLIBS="-L/usr/lib64/qt-3.3/lib -lqt-mt "
#export G4VIS_BUILD_OPENGLQT_DRIVER=
#export G4VIS_USE_OPENGLQT=

#ROOT on CUE
# Setup ROOT -- do this:  "source setroot_CUE"
# For sh family:
export ROOTSYS=/u/apps/root/5.26-00-64bit/root
#export ROOTSYS=/u/apps/root/5.26-00/root
export PATH=$ROOTSYS/bin:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH

compilerDir=/u/apps/gcc/4.4.4/lib
#gcc 
export PATH=$HOME/bin:$PATH
export LD_LIBRARY_PATH=$HOME/lib:$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH

export CVS_ROOT=/group/hallc/cvsroot

clear

echo "Logged on to machine ${HOSTNAME}"
