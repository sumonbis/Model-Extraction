# Modeling QT Applications using LLM

QT applications use the signals-and-slots mechanisms for callback functionality. The objects can independently emit signals and the slots can react to certain emitted signals. While such design provide richer and type-safe connections between GUI components, cyclic connections may raise infinite loops and result in segfault.

The goal is to detect such cyclic signals and slots connections. In this project, we model the source code and extract models that show cycles. We leveraged LLM to create the models. 

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