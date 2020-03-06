# Get a file from tar archive file(may be compressed).
tar xvpf docker-18.09.2-x86_64-1_SBo.tar etc/rc.d/rc.docker.new
tar xvpf docker-18.09.2-x86_64-1_SBo.tar,gz etc/rc.d/rc.docker.new

# Delete a file from tar archive file.
tar --delete install/doinst.sh -vf docker-18.09.2-x86_64-1_SBo.tar

# Add a file to tar archive file( may be same name with multi file).
tar rvf docker-18.09.2-x86_64-1_SBo.tar install/doinst.sh

# Update a file to tar archive file(replace the same name file).
nope

#end
