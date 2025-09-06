import logging
import os


class Config:
    BASE_CMD = os.path.normpath(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    )

    RELATIVE_COMPILER_DIR = "compiler"
    COMPILER_DIR = os.path.normpath(os.path.join(BASE_CMD, RELATIVE_COMPILER_DIR))
    COMPILER_VENV_DIR = os.path.normpath(os.path.join(COMPILER_DIR, "venv"))
    COMPILER_REQUIREMENTS_FILE = os.path.normpath(
        os.path.join(COMPILER_DIR, "requirements.txt")
    )

    RELATIVE_C_COMPILER_DIR = "c-compiler"
    C_COMPILER_DIR = os.path.normpath(os.path.join(BASE_CMD, RELATIVE_C_COMPILER_DIR))

    MACHINE_DIR = os.path.normpath(os.path.join(BASE_CMD, "machine"))

    LOGGER_NAME = "GPL-BUILD"

    LOG_LEVEL = logging.DEBUG

    BUILD_EXIT_ERROR_CODE = 1000
