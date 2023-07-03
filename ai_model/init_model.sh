#!/bin/bash

MODEL_NAME=det_body_ssd_mobilenetv1_v2.0.1
MODEL_DIR=model
INSTALL_DIR=/home/image-map-censor

AI_MODEL_NAME=${INSTALL_DIR}/ai_model/${MODEL_NAME}
UBUNTU_CUDA10=${INSTALL_DIR}/lib/ubuntu-cuda10

cd ${INSTALL_DIR}/ai_model

GPU_TYPE=`nvidia-smi  -L`
echo $GPU_TYPE 1>&1

REAL_MODEL_NAME=''
if [[ $GPU_TYPE == *"Tesla P40 "* ]]; then
    echo 'Find P40' >&1
    REAL_MODEL_NAME="${MODEL_NAME}.trt-P40"
elif [[ $GPU_TYPE == *"GeForce GTX 745"* ]]; then
    echo 'Find GeForce GTX 745' >&1
    REAL_MODEL_NAME="${MODEL_NAME}.trt-GTX745"
else
    echo "Find new nivdia: ${GPU_TYPE}" >&1
    echo "alertcode:91001009,alertmsg:new nvidia ${GPU_TYPE}" >&1

    export LD_LIBRARY_PATH=libs/linux/gpu/:/usr/local/cuda/lib64:${UBUNTU_CUDA10}:${LD_LIBRARY_PATH}
    libs/linux/gpu/performance_testing_GPU ${MODEL_DIR}/${MODEL_NAME}.uff 1 1 1 1
    REAL_MODEL_NAME=${MODEL_NAME}.trt
fi

echo "real trt_file_name: ${REAL_MODEL_NAME}"
if [ ! -f "${MODEL_DIR}/${REAL_MODEL_NAME}" ]; then
    echo "Not found ${MODEL_DIR}/${REAL_MODEL_NAME}"
    export LD_LIBRARY_PATH=libs/linux/gpu/:/usr/local/cuda/lib64:${UBUNTU_CUDA10}:${LD_LIBRARY_PATH}
    chmod a+x libs/linux/gpu/performance_testing_GPU
    libs/linux/gpu/performance_testing_GPU ${MODEL_DIR}/${MODEL_NAME}.uff 1 1 1 1
    REAL_MODEL_NAME=${MODEL_NAME}.trt
fi

cp ${MODEL_DIR}/${REAL_MODEL_NAME} ${AI_MODEL_NAME}.trt
