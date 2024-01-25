import sys
import openai
import signal
import os
import traceback
import time
import traceback
import tiktoken
import re
import graphviz

class TimeoutError(Exception):
    pass

def timeout_handler(signum, frame):
    raise TimeoutError()

class ChatGPTCall(object):
    def __init__(self, api_key_file="api_key.txt", model_name="gpt-3.5-turbo"):
    # def __init__(self, api_key_file="api_key.txt", model_name="gpt-4"):
        self.api_key = self.load_api(api_key_file)
        self.model_name = model_name

    @staticmethod
    def load_api(api_key_file):
        if not os.path.exists(api_key_file):
            raise ValueError(
                f"API key not found: {api_key_file}.")
        with open(api_key_file, "r") as file:
            api_key = file.read().split("\n")[0]
        return api_key

    def ask_gpt(self, query):
        openai.api_key = self.api_key
        try:
            res = openai.ChatCompletion.create(
                model=self.model_name,
                messages=[
                    # {"role": "system", "content": "You are a helpful assistant."},
                    # {"role": "user", "content": "Previous Prompts"},
                    # {"role": "user", "content": "Previous Queries"},
                    {"role": "system", "content": "You are a helpful assistant that understands C++ code and software architecture."},
                    {"role": "user", "content": f"{query}"},
                ],
                # temperature=0
            )
            res = res.choices[0].message.content
        except TimeoutError:
            signal.alarm(0)  # reset alarm
            return "TIMEOUT, No response from GPT for 5 minutes"
        except Exception:
            print(traceback.format_exc())
            print("Error during querying, sleep for one minute")
            time.sleep(60)  # sleep for 1 minute
            res = self.ask_gpt(query)
        return res

    def query(self, query, timeout=60*5):
        """
        :param query: the query to ask GPT
        :param timeout: the timeout for the user's query
        :return: the response from GPT
        """
        signal.signal(signal.SIGALRM, timeout_handler)
        signal.alarm(timeout)  # set timeout to 5 minutes
        try:
            response = self.ask_gpt(query)
        except TimeoutError:
            print("TIMEOUT ERROR!!!")
            return "TIMEOUT, No Response From GPT For 5 minutes"
        finally:
            signal.alarm(0)  # reset alarm
        return response


def test_timeout():
    chatGPTCall = ChatGPTCall()
    res = chatGPTCall.query("noop", timeout=1)
    assert res is None
    print("Test Timeout Pass!")

def get_answer(q):
    chatGPTCall = ChatGPTCall()
    res = chatGPTCall.query(q)
    # assert res is None
    return res


def test_api_key():
    try:
        chatGPTCall = ChatGPTCall("fake_path")
    except:
        print("Test Invalid API Key Pass")
    else:
        raise ValueError("Test Fail For Invalid API Key!")

def load_query(path):
        if not os.path.exists(path):
            raise ValueError(
                f"The File Is Not Found: {path}.")
        with open(path, "r") as file:
            query = file.read()
        return query

def write_to_file(file_path, text):
    try:
        with open(file_path, "w+") as file:
            file.write(text)
        print("Saved to file.")
    except IOError:
        print("An error occurred while writing to the file.")


def truncate_to_token_limit(text, token_limit):
    # enc = tiktoken.get_encoding("p50k_base")
    enc = tiktoken.encoding_for_model("gpt-3.5-turbo")
    # enc = tiktoken.encoding_for_model("gpt-4")
    tokens = enc.encode(text)

    if len(tokens) <= token_limit:
        return text
    
    truncated_tokens = tokens[:token_limit]
    print(len(truncated_tokens))
    truncated_text = enc.decode(truncated_tokens)
    print("@@@ Truncated to token limit.")
    return truncated_text

def get_P_model(summary):
    arch_cmd = 'Translate the following C code to P language. Ensure that the generated P code follows the state machine description:\n'
    print('Generating P model ...')
    aadl_model = get_answer(arch_cmd + summary)
    # aadl_model = get_answer(arch_cmd + "implementation code: \n " + code + "\n Summary:\n" + summary)
    return aadl_model


def dot_from_src(file_name):
    arch_cmd = 'Identify the signal-slot connections between the objects from the code below. Write the graph in DOT language where the nodes are objects and edges are the connections. The node label should include the internalValue state of the object. The edge label should include the connection method used ("connect" or "disconnect").\n'
    code = load_query(file_name)
    # code = truncate_to_token_limit(code, 3000)
    print('Generating DOT code ...')
    arch_desc = get_answer(arch_cmd + code)
    return arch_desc

def extract_code(text):
    pattern = r'```(.*?)```'
    match = re.search(pattern, text, re.DOTALL)
    
    if match:
        code = match.group(1)
        return code.strip()
    else:
        return None

def generate_and_save_dot_to_pdf(output_pdf_path, dot_source):
    # Create a Graphviz object
    graph = graphviz.Source(dot_source, format='pdf')

    # Render the graph to a PDF file
    graph.render(output_pdf_path, cleanup=True)

from pygccxml import utils, declarations
from pygccxml.parser import parse

def extract_functions_from_cpp(file_path):
    # Parse the C++ code
    decls = parse([file_path], config=utils.xml_generator_configuration_t())

    # Extract functions from the AST
    functions = [decl for decl in declarations.get_global_declarations(decls) if isinstance(decl, declarations.calldef_t)]


    return functions

if __name__ == "__main__":
    arguments = sys.argv

    if len(arguments) > 1:
        directory_path = arguments[1]
    else:
        print("No argument provided.")

    subdirectories = [entry for entry in os.listdir(directory_path) if os.path.isdir(os.path.join(directory_path, entry))]

    # tests = extract_functions_from_cpp(arguments[2])
    # for test in tests:
    #     print(test)

    # Iterating through the immediate subdirectories
    for subdirectory in subdirectories:
        subdirectory_path = os.path.join(directory_path, subdirectory)
        
        # Using os.listdir() again to list the files within the subdirectory
        files_in_subdirectory = [file for file in os.listdir(subdirectory_path) if os.path.isfile(os.path.join(subdirectory_path, file))]
        
        for file_name in files_in_subdirectory:
            if(file_name.endswith(".cpp")):
                print(f"Files in {subdirectory_path}:")
                print(file_name)

                # Generate digraph in one-shot
                # Step 1
                response = dot_from_src(os.path.join(subdirectory_path, file_name))
                dot_code = extract_code(response)
                if dot_code.startswith("dot") or dot_code.startswith("DOT"):
                    dot_code = dot_code[3:].lstrip()
                write_to_file(subdirectory_path + "/" + file_name + "_response.txt", response)
                write_to_file(subdirectory_path + "/" + file_name + "_dot.txt", dot_code)
                generate_and_save_dot_to_pdf(subdirectory_path + "/" + file_name + "_digraph", dot_code)
                print("====================================")