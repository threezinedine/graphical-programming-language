import logging
from .config import Config

logger = logging.getLogger(Config.LOGGER_NAME)
formatter = logging.Formatter(
    "[%(levelname)s] - [%(name)s] - %(filename)s:%(lineno)d - %(message)s"
)
std_handler = logging.StreamHandler()
std_handler.setFormatter(formatter)
logger.addHandler(std_handler)
logger.setLevel(Config.LOG_LEVEL)
