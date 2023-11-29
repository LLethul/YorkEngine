# make --always-make --dry-run \
#  | grep -wE 'gcc|g\+\+|c\+\+' \
#  | grep -w '\-c' \
#  | jq -nR '[inputs|{directory:".", command:., file: match(" [^ ]+$").string[1:]}]' \
#  > compile_commands.json

# PowerShell equivalent:

# Run the 'make' command with the options '--always-make' and '--dry-run'
# Redirect the output to a temporary file
make --always-make --dry-run > temp.txt

# Filter the lines in the temporary file that contain 'gcc', 'g++', or 'c++'
# Redirect the filtered output to another temporary file
Select-String -Pattern 'gcc|g\+\+|c\+\+' temp.txt > temp2.txt

# Filter the lines in the second temporary file that contain '-c'
# Redirect the filtered output to another temporary file
Select-String -Pattern '\-c' temp2.txt > temp3.txt

# Use 'jq' to format the input as a JSON array of objects
# The objects have properties 'directory', 'command', and 'file'
# The 'file' property is extracted from the input using a regular expression
# Redirect the formatted output to the file 'compile_commands.json'
Get-Content temp3.txt | ForEach-Object {
    $directory = "."
    $command = $_
    $file = ($_ -split " ")[-1]
    [PSCustomObject]@{
        directory = $directory
        command = $command
        file = $file
    }
} | ConvertTo-Json > compile_commands.json

# Clean up the temporary files
Remove-Item temp.txt, temp2.txt, temp3.txt