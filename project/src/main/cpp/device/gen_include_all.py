import os
import sys

def gen(directory,file_path):
    subdirectory_list = os.listdir(directory)
    
    all_cl_files = []
    for sub in subdirectory_list:
        sub_dir = '{}/{}'.format(directory,sub)
        if not os.path.isdir(sub_dir):
            continue
        cl_files = os.listdir(sub_dir)
        cl_files = map(lambda x:'#include "{}/{}"'.format(sub_dir,x),cl_files)
        all_cl_files.extend(cl_files)
    all_cl_files = sorted(all_cl_files)
    file_content = '\n'.join(all_cl_files)
    with open(file_path,'w') as f:
        f.write(file_content)
    print('Kernel files in directory "{}" are included into "{}" successfully.'.format(directory,file_path))
    
usage = """
This script is used to include all cl files to a single cl file.
usage:
    python *this_file*
    ( equals to
        python *this_file* kernels aclmkl.cl )
    or
    python *this_file* KERNELS_DIRECTORY TARGET_PATH
examples:
    python *this_file* kernels aclmkl.cl   
"""
def main():
    args = len(sys.argv)
    if args == 1:
        kernels_root = 'kernels'
        target_path = 'aclmkl.cl'
    elif args == 3:
        kernels_root = 'kernels'
        target_path = 'aclmkl.cl'
    else:
        print(usage)
        exit(-1)
    gen(kernels_root,target_path)

if __name__ == '__main__':
    main()
