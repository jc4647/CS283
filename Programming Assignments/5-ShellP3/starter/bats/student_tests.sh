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

@test "Pipes paresed" {
    run ./dsh <<EOF                
cd | cd
exit
EOF
    # Assertions
    [ "$status" -eq 0 ]
}

@test "Exit Command" {
    run ./dsh <<EOF                
exit
EOF
    # Assertions
    [ "$status" -eq 0 ]

}

@test "No command provided" {
    run ./dsh <<EOF                

exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>warning:nocommandsprovideddsh3>"

    echo "Captured stdout:" 
    echo "Output: $stripped_output"
    echo "Expected: $expected_output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "Check if cd changes directory successfully" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [ "$status" -eq 0 ]
    
    result=$(echo "$output" | grep -E '^/' | tail -n1)
    echo "Extracted result: $result"  
    [[ "$result" == "/tmp" ]]
}

@test "Check if exit terminates the shell correctly" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Check if invalid command returns an error" {
    run ./dsh <<EOF
invalidcommand
EOF
    [[ "$output" =~ "Execvp failed" ]] 
}

@test "Check if whoami runs correctly" {
    run ./dsh <<EOF
whoami
EOF
    [ "$status" -eq 0 ]
}