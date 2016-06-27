#!/usr/bin/env bats

@test "Non Horn Clauses" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'Non Horn Clauses: 0')"
  [ "$(echo $?)" -eq 0 ]
}

@test "Non Dual Horn Clauses" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'Non Dual Horn Clauses: 11')"
  [ "$(echo $?)" -eq 0 ]
}

@test "Facts" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'Facts: 0')"
  [ "$(echo $?)" -eq 0 ]
}

@test "Rules" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'Rules: 20')"
  [ "$(echo $?)" -eq 0 ]
}

@test "Constraints" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'Constraints: 20')"
  [ "$(echo $?)" -eq 0 ]
}

@test "max clause size" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'max clause size: 4')"
  [ "$(echo $?)" -eq 0 ]
}

@test "max positive clause size" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'max positive clause size: 4')"
  [ "$(echo $?)" -eq 0 ]
}

@test "max negative clause size" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'max negative clause size: 1')"
  [ "$(echo $?)" -eq 0 ]
}

@test "number of variables that occur as positive literals" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'with helpers: 20 without helpers: 13')"
  [ "$(echo $?)" -eq 0 ]
}

@test "number of variables that occur as negative literals" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'with helpers: 4 without helpers: 0')"
  [ "$(echo $?)" -eq 0 ]
}

@test "maximum positive rule size constraint" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'maximum positive rule size constraint: 4')"
  [ "$(echo $?)" -eq 0 ]
}

@test "maximum positive rule size non-constraint" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'maximum positive rule size non-constraint: 0')"
  [ "$(echo $?)" -eq 0 ]
}

@test "total number of atom occurrences constraint" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'total number of atom occurrences constraint: 48')"
  [ "$(echo $?)" -eq 0 ]
}

@test "total number of atom occurrences non-constraint" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'total number of atom occurrences non-constraint: 0')"
  [ "$(echo $?)" -eq 0 ]
}

@test "maximum number of occurrences of an atom" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'maximum number of occurrences of an atom: 5')"
  [ "$(echo $?)" -eq 0 ]
}

@test "maximum number of positive occurrences of an atom" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'maximum number of positive occurrences of an atom: 5')"
  [ "$(echo $?)" -eq 0 ]
}

@test "maximum number of negative occurrences of an atom" {
  result="$(./build/Debug/clasp --stats=2 -V ./Tests/data/graph_colouring_small.lp  | grep 'maximum number of negative occurrences of an atom: 1')"
  [ "$(echo $?)" -eq 0 ]
}
