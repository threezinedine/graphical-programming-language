import os
import sys


def pytest_configure():
    sys.path.append(os.path.dirname(os.path.abspath(__file__)))
