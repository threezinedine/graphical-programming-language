from config.args import Parser
from config.config import Config
from config.utils import AddPackage, CreateEnv, RunSpecTest, RunTest


def main() -> None:
    parser = Parser()
    args = parser.Args

    if args.command == "test":
        if args.project == "compiler":
            CreateEnv(Config.RELATIVE_COMPILER_DIR)
            RunTest(Config.RELATIVE_COMPILER_DIR)
        elif args.project == "spec":
            CreateEnv(Config.RELATIVE_COMPILER_DIR)
            RunSpecTest(Config.RELATIVE_COMPILER_DIR, args.spec)
    elif args.command == "add":
        if args.project == "compiler":
            CreateEnv(Config.RELATIVE_COMPILER_DIR)
            AddPackage(Config.RELATIVE_COMPILER_DIR, args.package)


if __name__ == "__main__":
    main()
