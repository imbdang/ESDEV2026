MIDDLE_DIRS := middle/m_clock \
			   middle/m_gpio \
			   middle/m_encoder \
			   middle/m_timer \
			   middle/m_sensor \
			   middle/m_mpu

COMPONENTS_SRCDIRS		+= $(MIDDLE_DIRS)
COMPONENTS_ADD_INCLUDES	+= $(MIDDLE_DIRS)