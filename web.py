from pyChatGPT import ChatGPT

#  auth with google login
api = ChatGPT(auth_type='google', email='sumon.it40@gmail.com', password='')


resp = api.send_message('Hello, world!')
print(resp['message'])

api.reset_conversation()  # reset the conversation
api.clear_conversations()  # clear all conversations
api.refresh_chat_page()  # refresh the chat page
