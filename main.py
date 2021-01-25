from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys
import time
import random
import poker_bot

game_id = "86660661064461116193"
random.seed(time.time)
username = "Big_Dump_" + str(random.randint(1, 999)) + "_Bot"
buy_in_amount = 20

driver = webdriver.Firefox()
driver.get("https://lipoker.io/game/" + game_id)

def login(username):
    input_box = WebDriverWait(driver, 10).until(
        EC.presence_of_element_located((By.ID, "input-username"))
    )
    input_box.send_keys(username + Keys.RETURN)

def buy_in(buy_in_amount):
    driver.implicitly_wait(4)
    input_box_buy_in_amount = WebDriverWait(driver, 10).until(
        # TODO: figure out how to make an "//input[@data-testid='buyin-field']" work instead of this abs path
        EC.presence_of_element_located((By.XPATH, "/html/body/div/div/div[5]/div[1]/div[9]/div[2]/div/input"))
    )
    input_box_buy_in_amount.send_keys(str(buy_in_amount) + Keys.RETURN)
    driver.find_element_by_xpath("/html").send_keys("B")

def wait_for_game_start():
    # will just loop thru all the buttons in the page (every 2 secs)
    # until we find one that says "check" or "call" or "fold"
    while True:
        buttons = driver.find_elements_by_tag_name("button")
        for button in buttons:
            print("cur button text: " + button.text.lower())
            if(button.text.lower() in ["check", "call", "fold"]):
                return
        time.sleep(2)

def wait_for_game_start_v2():
    """
    waits for the game to start.
    
    Returns the msg id of the "username bought in for buy_in_amount"
    """
    last_msgs_size = 0
    while True:
        msgs = driver.find_elements_by_xpath("//div[contains(@id,'chat-')]")
        if(len(msgs) > last_msgs_size):
            new_msgs = msgs[last_msgs_size:len(msgs)]
            for i in range(len(new_msgs)):
                if(username + " bought in" in new_msgs[i].text):
                    return last_msgs_size + i + 1
            last_msgs_size = len(msgs)
        time.sleep(2)


def handle_new_msgs(msgs):
    for msg in msgs:
        print(msg.text)

login(username)
buy_in(buy_in_amount)
#wait_for_game_start()

last_msgs_size = wait_for_game_start_v2()
while True:
    msgs = driver.find_elements_by_xpath("//div[contains(@id,'chat-')]")
    if(len(msgs) > last_msgs_size):
        handle_new_msgs(msgs[last_msgs_size:len(msgs)])
        last_msgs_size = len(msgs)
    time.sleep(2)

end = input("ENTER to quit")
driver.quit()

