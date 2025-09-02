import argparse


class Parser:
    def __init__(self) -> None:
        parser = argparse.ArgumentParser(description="Build and manage the project.")

        subparsers = parser.add_subparsers(dest="command", required=True)

        test = subparsers.add_parser("test", help="Run tests")
        test.add_argument(
            "project",
            choices=["compiler"],
            default="compiler",
            type=str,
        )

        add = subparsers = subparsers.add_parser(
            "add", help="Add new package using pip"
        )
        add.add_argument("project", choices=["compiler"], type=str)
        add.add_argument("package", type=str, help="Package name to add")

        self._args = parser.parse_args()

    @property
    def Args(self):
        return self._args
