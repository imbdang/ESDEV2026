HARDWARE_DIRS := hardware/motor \
                 hardware/register \
                 hardware/interrupt \
                 hardware/systick

COMPONENTS_SRCDIRS      += $(HARDWARE_DIRS)
COMPONENTS_ADD_INCLUDES += $(HARDWARE_DIRS)