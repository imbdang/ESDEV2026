MIDDLE_DIRS := middle/clock \
			   middle/gpio \
			   middle/uart \
			   middle/timer

COMPONENTS_SRCDIRS		+= $(MIDDLE_DIRS)
COMPONENTS_ADD_INCLUDES	+= $(MIDDLE_DIRS)