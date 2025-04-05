# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
*i*) ;;
*) return ;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
xterm-color | *-256color) color_prompt=yes ;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
        # We have color support; assume it's compliant with Ecma-48
        # (ISO/IEC-6429). (Lack of such support is extremely rare, and such
        # a case would tend to support setf rather than setaf.)
        color_prompt=yes
    else
        color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    if [ $(id -u) -eq 0 ]; then
        PS1='${debian_chroot:+($debian_chroot)}\[\033[01;31m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
    else
        PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
    fi
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm* | rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*) ;;

esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
    if [ -f /usr/share/bash-completion/bash_completion ]; then
        . /usr/share/bash-completion/bash_completion
    elif [ -f /etc/bash_completion ]; then
        . /etc/bash_completion
    fi
fi

# Function to update and get packages using apt-get
apt-get() {
    # Check if package cache is older than 24 hours
    if [ ! -e /var/cache/apt/pkgcache.bin ] || (($(date +%s) - $(stat -c %Y /var/cache/apt/pkgcache.bin) >= 24 * 60 * 60)); then
        # Update package list
        /usr/bin/apt-get update
    fi
    # Get packages
    /usr/bin/apt-get "$@"
}

# Function to update and get packages using apt
apt() {
    # Check if package cache is older than 24 hours
    if [ ! -e /var/cache/apt/pkgcache.bin ] || (($(date +%s) - $(stat -c %Y /var/cache/apt/pkgcache.bin) >= 24 * 60 * 60)); then
        # Update package list
        /usr/bin/apt update
    fi
    # Get packages
    /usr/bin/apt "$@"
}

# Export functions
export -f apt-get
export -f apt

# Function to check package cache
check_package_cache() {
    # Check if package cache has been checked
    if [ "$PACKAGE_CACHE_CHECKED" != "true" ]; then
        # Check if package cache is older than 24 hours
        if [ ! -e /var/cache/apt/pkgcache.bin ] || (($(date +%s) - $(stat -c %Y /var/cache/apt/pkgcache.bin) >= 24 * 60 * 60)); then
            local CMD_UPDATE
            # Check if user is root
            if [ "$(id -u)" != "0" ]; then
                CMD_UPDATE="sudo apt update"
            else
                CMD_UPDATE="apt update"
            fi
            # Display message based on language
            case ${LANG%%_*} in
            "de") echo -e "\033[1mAchtung:\033[0m Die Paketdatenbank ist veraltet.\nEs wird empfohlen, '\033[1m${CMD_UPDATE}\033[0m' vor der Installation von Software auszuführen." ;;
            "es") echo -e "\033[1mAtención:\033[0m La base de datos de paquetes está desactualizada.\nSe recomienda ejecutar '\033[1m${CMD_UPDATE}\033[0m' antes de instalar software." ;;
            "fr") echo -e "\033[1mAttention:\033[0m La base de données des paquets est obsolète.\nIl est recommandé d'exécuter '\033[1m${CMD_UPDATE}\033[0m' avant d'installer des logiciels." ;;
            "it") echo -e "\033[1mAttenzione:\033[0m Il database dei pacchetti è obsoleto.\nSi consiglia di eseguire '\033[1m${CMD_UPDATE}\033[0m' prima di installare il software." ;;
            "pt") echo -e "\033[1mAtenção:\033[0m A base de dados de pacotes está desatualizada.\nÉ recomendado executar '\033[1m${CMD_UPDATE}\033[0m' antes de instalar softwares." ;;
            "ru") echo -e "\033[1mВнимание:\033[0m База данных пакетов устарела.\nРекомендуется выполнить '\033[1m${CMD_UPDATE}\033[0m' перед установкой программ." ;;
            *) echo -e "\033[1mAttention:\033[0m The package database is outdated.\nIt is recommended to run '\033[1m${CMD_UPDATE}\033[0m' before installing software." ;;
            esac
        fi
        # Mark package cache as checked
        export PACKAGE_CACHE_CHECKED=true
    fi
}

# Set prompt command to check package cache
PROMPT_COMMAND="check_package_cache"
