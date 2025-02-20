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

@test "Echo command" {
    run ./dsh <<EOF
echo Hello, World!
EOF

    expected_output="Hello, World!"

    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "Change directory and Check pwd" {
    run ./dsh <<EOF 
cd /tmp
pwd
EOF

    expected_output="/tmp"


    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "Starts and Exits" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Quoted arguments" {
    run ./dsh <<EOF
echo "Hello BATS"
EOF

    expected_output="Hello BATS"

    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "Check environment variables are listed" {
    run ./dsh <<EOF
env
EOF

    expected_output="HOME="

    [ "$status" -eq 0 ]
    [[ "$output" == *"$expected_output"* ]]
}

@test "Error rc command" {
    run ./dsh <<EOF
cd /invalid/path
rc
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"2"* ]] 
}

@test "Success rc command" {
    run ./dsh <<EOF
ls
rc
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"0"* ]]
}

@test "Test cd with too many arguements" {
    run ./dsh <<< "cd dir1 dir2"
    [ "$status" -eq 0 ]
    [[ "$output" == *"cd: Too many arguments"* ]]
}