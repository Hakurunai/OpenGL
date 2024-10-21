Got an issue to publish from visual Studio, use cli from project root to generate the output
PS : the output is founded in the project\bin, for an obscure reason i cannot have a publish
directory filled, and visual is only able to create a self-contained application -> too size heavy

dotnet publish -c Release -r osx-x64 -f net8.0-macos --self-contained false --output ./publish