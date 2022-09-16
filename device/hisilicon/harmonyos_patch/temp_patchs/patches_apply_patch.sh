#
# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#!/bin/bash
set -e

PATCH_DIR_REL=device/hisilicon/harmonyos_patch/temp_patchs
ROOT_PATH=${PWD}
PATCH_DIR_ABS=${ROOT_PATH}/${PATCH_DIR_REL}

PATCH_FILE_LIST=$(find ${PATCH_DIR_ABS} -name "*.patch")

# Generate file list and info list
index=0
for F in ${PATCH_FILE_LIST[@]}; do
    REL_F=${F##*/temp_patchs/}
    DIR_F=${REL_F%/*}

    PATCH_FILE_LIST[${index}]="${PATCH_INFO_LIST} ${DIR_F},$F"
    ((++index))
done
# Sorting by target path, to be easy to do git operations.
PATCH_INFO_LIST=($(echo ${PATCH_FILE_LIST[@]} | tr ' ' '\n' | sort))

# Apply patches
cur_path=${PATCH_DIR_ABS}
for I in ${PATCH_INFO_LIST[@]}; do
    patch_dir=${I%,*}
    patch_file=${I##*,}
    target_dir=${ROOT_PATH}/${patch_dir}

    if [ -d "${target_dir}"/.git ]; then
        cd ${target_dir}

        # Whether to perform the hard-reset operation.
        if [[ "${cur_path}" != "${target_dir}" ]]; then
            cur_path=${target_dir}
            branch_name=$(git branch -r | grep "\->" | awk '{print $3}')
            git reset --hard ${branch_name} > /dev/null
        fi

        # Do apply current patch
        git am --reject --whitespace=fix ${patch_file}
        if [ $? -eq 0 ]; then
            echo "Success."
        else
            # Retry once more
            echo "Failed, retry for once more."
            git am --abort
            git reset --hard ${branch_name} > /dev/null
            git am -3 ${patch_file}
            if [ $? -eq 0 ]; then
                echo "Success."
            else
                echo "Fail to apply patch: ${patch_file}"
            fi
        fi
    fi
done
