import json
import openai
import signal
import os
import traceback
import time
import traceback


class TimeoutError(Exception):
    pass


def timeout_handler(signum, frame):
    raise TimeoutError()


class ChatGPTCall(object):
    def __init__(self, api_key_file="api_key.txt", model_name="gpt-3.5-turbo"):
        self.api_key = self.load_api(api_key_file)
        self.model_name = model_name

    @staticmethod
    def load_api(api_key_file):
        if not os.path.exists(api_key_file):
            raise ValueError(
                f"The API Key File Is Not Found: {api_key_file}. Please Create It And Store Your API Key Here.")
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
                    {"role": "user", "content": f"{query}"},
                ]
            )
            res = res.choices[0].message.content
        except TimeoutError:
            signal.alarm(0)  # reset alarm
            return "TIMEOUT, No Response From GPT For 5 minutes"
        except Exception:
            print(traceback.format_exc())
            print("Error During Querying, Sleep For One Minute")
            time.sleep(60)  # sleep for 1 minute
            res = self.ask_gpt(query)
        return res

    def query(self, query, timeout=60*5):
        """
        The query function for user to send query to the chatGPT
        :param query: the user's prompt
        :param timeout: the timeout for the user's query
        :return: the GPT's response
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

def test_query(q):
    chatGPTCall = ChatGPTCall()
    res = chatGPTCall.query(q)
    # assert res is None
    print(res)


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
                f"The API Key File Is Not Found: {api_key_file}. Please Create It And Store Your API Key Here.")
        with open(path, "r") as file:
            query = file.read()
        return query

if __name__ == "__main__":
    # test class constructor
    # test_timeout()
    # test_api_key()
    query = load_query('q.txt')
    # print(query)
    test_query(query)
