import os
import subprocess

from .config import Config
from .logger import logger

##### All path inside this util methods are relative with the base cwd in config


def CreateFolder(folder: str) -> None:
    if os.path.exists(folder):
        logger.debug(f"Folder '{os.path.normpath(folder)}' already exists.")

    try:
        os.makedirs(folder, exist_ok=True)
    except Exception as e:
        logger.error(f"Failed to create folder '{os.path.normpath(folder)}': {e}")
        exit(Config.BUILD_EXIT_ERROR_CODE)


def CreateNestedFolder(folder: str) -> None:
    folders = os.path.split(folder)

    for i in range(1, len(folder)):
        path = os.path.join(*folders[: i + 1])
        try:
            os.makedirs(path, exist_ok=True)
        except Exception as e:
            logger.error(f"Failed to create folder '{os.path.normpath(path)}': {e}")
            exit(Config.BUILD_EXIT_ERROR_CODE)


def GetTempFile(file: str) -> str:
    return os.path.join("temp", file)


def IsFileUpToDate(file: str) -> bool:
    """
    Assume that the file exist.
    """
    tempFile = GetTempFile(file)

    if not os.path.exists(tempFile):
        return False

    if os.path.getmtime(file) > os.path.getmtime(tempFile):
        return False

    return True


def GetPythonExePath(folder: str) -> str:
    return os.path.join(Config.BASE_CMD, folder, "venv", "Scripts", "python.exe")


def GetPytestExePath(folder: str) -> str:
    return os.path.join(Config.BASE_CMD, folder, "venv", "Scripts", "pytest.exe")


def TouchFile(filePath: str) -> None:
    tempFile = GetTempFile(filePath)
    folder = os.path.dirname(tempFile)
    CreateNestedFolder(folder)

    with open(tempFile, "w") as file:
        file.write("")


def CreateEnv(folder: str) -> None:
    isRequirementsUpToDate = IsFileUpToDate(os.path.join(folder, "requirements.txt"))
    isVenvExists = os.path.exists(os.path.join(folder, "venv"))

    if isRequirementsUpToDate and isVenvExists:
        logger.debug(f"Virtual environment in '{folder}' is up to date.")
        return

    if not isVenvExists:
        try:
            logger.info(f'Folder "venv" does not exist, creating ...')
            subprocess.run(
                [
                    "python",
                    "-m",
                    "venv",
                    "venv",
                ],
                check=True,
                shell=True,
                cwd=folder,
            )
            subprocess.run(
                [
                    GetPythonExePath(folder),
                    "-m",
                    "pip",
                    "install",
                    "--upgrade",
                    "pip",
                ],
                check=True,
                shell=True,
                cwd=folder,
            )
            logger.info(f'Folder "venv" created successfully.')
        except Exception as e:
            logger.error(f"Failed to create folder 'venv': {e}")
            exit(Config.BUILD_EXIT_ERROR_CODE)

    if not isRequirementsUpToDate:
        if not os.path.exists(os.path.join(folder, "requirements.txt")):
            logger.warning(
                f'Requirements file "requirements.txt" does not exist in "{folder}". Skipping...'
            )
            return

        try:
            logger.info(f'Updating requirements in "{folder}"...')
            subprocess.run(
                [
                    GetPythonExePath(folder),
                    "-m",
                    "pip",
                    "install",
                    "-r",
                    "requirements.txt",
                ],
                check=True,
                shell=True,
                cwd=folder,
            )
            TouchFile(os.path.join(folder, "requirements.txt"))
            logger.info(f'Updated requirements in "{folder}".')
        except Exception as e:
            logger.error(f"Failed to update requirements in '{folder}': {e}")
            exit(Config.BUILD_EXIT_ERROR_CODE)


def AddPackage(folder: str, package: str) -> None:
    try:
        logger.info(f"Adding package '{package}' in '{folder}'...")
        subprocess.run(
            [
                GetPythonExePath(folder),
                "-m",
                "pip",
                "install",
                package,
            ],
            check=True,
            shell=True,
            cwd=folder,
        )
        subprocess.run(
            [
                GetPythonExePath(folder),
                "-m",
                "pip",
                "freeze",
                ">",
                "requirements.txt",
            ],
            check=True,
            shell=True,
            cwd=folder,
        )
        TouchFile(os.path.join(folder, "requirements.txt"))
        logger.info(f"Package '{package}' added successfully in '{folder}'.")
    except Exception as e:
        logger.error(f"Failed to add package '{package}' in '{folder}': {e}")
        exit(Config.BUILD_EXIT_ERROR_CODE)


def RunTest(folder: str) -> None:
    try:
        logger.info(f"Running tests in '{folder}'...")
        subprocess.run(
            [
                GetPytestExePath(folder),
            ],
            check=True,
            shell=True,
            cwd=folder,
        )
        logger.info(f"Tests in '{folder}' completed successfully.")
    except Exception as e:
        logger.error(f"Failed to run tests in '{folder}': {e}")
        exit(Config.BUILD_EXIT_ERROR_CODE)
