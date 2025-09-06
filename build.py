from config.args import Parser
from config.config import Config
from config.utils import (
    AddPackage,
    CreateEnv,
    RunSpecTest,
    RunTest,
    ConfigureCCompiler,
    BuildCCompiler,
    RunCCompilerTest,
)


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
        elif args.project == "c-compiler":
            BuildCCompiler()
            RunCCompilerTest()
    elif args.command == "add":
        if args.project == "compiler":
            CreateEnv(Config.RELATIVE_COMPILER_DIR)
            AddPackage(Config.RELATIVE_COMPILER_DIR, args.package)
    elif args.command == "build":
        if args.project == "c-compiler":
            ConfigureCCompiler()
            BuildCCompiler()


if __name__ == "__main__":
    main()
