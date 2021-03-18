#! /bin/bash

#
# Script should download and install the following open source projects:
#       - patchelf
#

##
# 
tmp_folder="/tmp"
# Changed by Instigate Robotics as link doesn't work.
#package_url="http://hydra.nixos.org/build/114505/download/2"
#package_name="patchelf-0.5"
package_url="https://nixos.org/releases/patchelf/patchelf-0.9/"
package_name="patchelf-0.9"
		
##
# The install path of OS3 project.
PREFIX=/opt/instigate/$project_name/$project_version

##
# The prefix URL of sourceforge.
SOURCEFORGE_LINK="https://sourceforge.net/projects"

##
# Checks the user privileges whether super use.
check_user_priviledges()
{
        test `whoami` != "root" && 
	echo "Please re-run script as root." && exit 2
}

# Prints out OS information
print_os_info()
{
        echo `cat /etc/issue` " "`uname -m`
        return 0
}

# Check and print operation system information
check_os()
{
	echo -n "Checking Operation System version ... "
	print_os_info
}

##
# Installs given package on system.
# Now it works only for Ubuntu
##
install()
{
	echo "Do you want to install $1? Y/N"
	read n
	test "$n" != "Y" && test "$n" != "y"  && exit 1
	apt-get install $1
}

##
# Checks for existence of given package. If not fount tries to install it.
##
check_for()
{
        echo -n "Checking existence of $1 ..."
        test "`which $1`" == "" && echo "   FAIL" && install $1 || echo "  found"
}

# Checks all preconditions needed for installation:
#       - script must be run with root privileges
check_preconditions()
{
	echo ""
	check_user_priviledges
	check_os
	check_for tar
	check_for g++
	check_for lcov
	check_for qemu
}

# Installs specified package
install_patchelf_package()
{
        echo -n "Checking existence of patchelf ..."
        `which patchelf > /dev/null` 
        if [ $? -ne 0 ]; then
                echo "FAIL"
                echo "Installing patchelf ... "
                root_folder=$PWD
                cd $tmp_folder
                wget $package_url/$package_name.tar.bz2 
                tar xf $package_name.tar.bz2 || exit 1
                cd $package_name || exit 1
                ./configure || exit 1
                make || exit 1
                make install || exit 1
                rm -rf /tmp/patchelf-0.5*
                cd $root_folder
                echo "done"
        else 
                echo "  found"
        fi
        echo ""
}

# Entry point of the script
main()
{
	mkdir -p $PREFIX
	check_preconditions
	install_patchelf_package
}

main $1
