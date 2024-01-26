# Results for LLM based model extraction

We manually investigated whether the extracted model could detect the Sig0Slot cycles. 
Total tests: 22
Passed tests: 15

## Regression tests 

1. Fail (too small model)
2. Fail (too small model)
3. Fail (too small model) 
4. Pass (but the connections are not correct)
5. Pass
6. Pass
7. Pass (but missing connection)
8. Fail (destroyed object is not considered)
9. Fail (the signal names are not considered)
10. Fail (did not consider the signal name)
11. Pass


## Enhancement tests

1. Pass
2. Pass
3. Pass
4. Pass
5. Pass
6. Pass
7. Pass
8. Fail
9. Pass
10. Pass
11. Pass

## Automatic conversion from DOT to Formula model

Worked for almost all of them. Results are in /tests/ directory. 