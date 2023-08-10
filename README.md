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