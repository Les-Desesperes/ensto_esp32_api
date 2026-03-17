Import("env")
import os

env_file = ".env"

if os.path.exists(env_file):
    print("Loading environment variables from .env file...")
    with open(env_file, "r") as f:
        for line in f:
            line = line.strip()
            # Ignore blank lines and comments
            if not line or line.startswith("#"):
                continue

            # Split the line into Key and Value
            key, value = line.split("=", 1)

            # Clean up any rogue quotes the user might have typed
            value = value.strip(' "\'')

            # Append as a C++ Macro. StringifyMacro ensures it gets wrapped in quotes for C++
            env.Append(CPPDEFINES=[(key, env.StringifyMacro(value))])
else:
    print("Warning: No .env file found!")