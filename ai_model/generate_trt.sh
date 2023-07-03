export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libs/linux/gpu/

# Set body box model
TF_MODEL_BOX=model/det_body_ssd_mobilenetv1_v2.0.1.uff

# Set test data
IMAGES_PATH=images/testing/

# Set test data
CONFIG_PATH=model/config.ini


libs/linux/gpu/performance_testing_GPU $TF_MODEL_BOX $IMAGES_PATH $CONFIG_PATH





