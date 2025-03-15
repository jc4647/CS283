#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "basic command execution (ls)" {
    run ./dsh -c -i 127.0.0.1 -p 7890 <<EOF
ls
exit
EOF
    [ "$status" -eq 0 ]
    [ -n "$output" ]
}

@test "long-running command handling" {
    run ./dsh -c -i 127.0.0.1 -p 7890 <<EOF
sleep 5
exit
EOF
    [ "$status" -eq 0 ]
}

@test "multiple consecutive commands" {
    run ./dsh -c -i 127.0.0.1 -p 7890 <<EOF
ls
pwd
exit
EOF
    [ "$status" -eq 0 ]
    [ -n "$output" ]
}

@test "Pipe with multiple commands: ls | sort | head -n 3" {
    run ./dsh <<EOF
ls | sort | head -n 3
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dsh" ]]
}

@test "Empty line handling" {
    run ./dsh <<EOF

ls
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dshlib.c" ]]
}

@test "server connection" {
    run ./dsh -c -i 127.0.0.1 -p 7890 <<EOF
ls
exit
EOF
    echo "Status: $status"
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dshlib.c" ]]
}

@test "large input command" {
    run ./dsh <<EOF
seq 1000 | head -n 50
EOF
    echo "Status: $status"
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "50" ]]
}

@test "Test rc command with success status" {
    run ./dsh <<EOF
ls
rc
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"0"* ]]
}

@test "command exit code" {
    run ./dsh <<EOF
ls
echo \$?
EOF
    echo "Status: $status"
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "0" ]]
}

@test "commands with special characters" {
    run ./dsh <<EOF
echo "Hello, World!" && echo "Goodbye, World!"
EOF
    echo "Status: $status"
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Hello, World!" ]]
}

@test "Consecutive pipes with different commands: seq 10 | grep 5 | xargs -I {} echo Found: {}" {
    run ./dsh <<EOF
seq 10 | grep 5 | xargs -I {} echo Found: {}
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Found: 5" ]]
}

@test "Complex pipe with wc: ls -l | grep '\.c$' | wc -l" {
    run ./dsh <<EOF
ls -l | grep '\.c$' | wc -l
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ [0-9]+ ]]
}