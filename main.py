from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys

game_id = "86660661064461116193"
username = "Big_Dump_11_Bot"
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
        # TODO: figure out how to make
        EC.presence_of_element_located((By.XPATH, "/html/body/div/div/div[5]/div[1]/div[9]/div[2]/div/input"))
    )
    input_box_buy_in_amount.send_keys(str(buy_in_amount) + Keys.RETURN)
    driver.find_element_by_xpath("/html").send_keys("B")



login(username)
buy_in(buy_in_amount)
# wait_for_buy_in_accepted()

end = input("press anything to quit")
