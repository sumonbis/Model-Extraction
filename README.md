# Model Extraction using LLM

This repository contains the source code that use OpenAI API to generate models from implementation code. Give an implementation of UAV application (e.g., C++ code), we generate an intermediate representation (IR) of the code using LLM. Then the IR is again used to generate the model using LLM. 

The [example](examples/) directory is a corpus of MAVSDK applications for UAV in C++. Each subdirectory contains the generated IR and corresponding AADL model. We are using gpt-3.5-turbo openai LLM for the generation. 

## Installation

Follow these steps to create a virtual environment.

1. Clone this repository. 

2. Run this on command line to create a virtual environment:

```
python3 -m venv llmenv
source llmenv/bin/activate
```
Run the following command to update pip on Python: `python3 -m pip install --upgrade pip`.

3. Navigate to the repository and install required packages:
```
pip install -r requirements.txt
```

4. Run the `python extract-model examples` to generate the AADL models for the MAVSDK code examples. The generated models are stored in the corresponding subdirectories.

Before running the command, place the openai key (`api_key.txt`) in the root directory of the repository.

## References

How to verify the correctness of generated AADL model:

- Johnsen, Andreas, Paul Pettersson, and Kristina Lundqvist. "An architecture-based verification technique for AADL specifications." European Conference on Software Architecture. Berlin, Heidelberg: Springer Berlin Heidelberg, 2011.

- Johnsen, Andreas, et al. "Automated verification of AADL-specifications using UPPAAL." 2012 IEEE 14th International Symposium on High-Assurance Systems Engineering. IEEE, 2012.

- Ling, Dongyi, et al. "Reliability evaluation based on the AADL architecture model." Journal of Networks 9.10 (2014): 2721.