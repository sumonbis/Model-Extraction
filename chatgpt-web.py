from chatgpt_wrapper import ChatGPT

bot = ChatGPT()
# query = "Hello, world! Are you gpt 3.5?"


def wrap_chatgpt(query="hello"):
    success, response, message = bot.ask(query)
    if success:
        # print("Q:", query[:50])
        # print("A:", response[:50])
        return response
    else:
        raise RuntimeError(message)
