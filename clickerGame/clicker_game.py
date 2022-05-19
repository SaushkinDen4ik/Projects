from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.core.window import Window
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty

Window.size = (600, 400)
Window.clearcolor = (10/255, 0/255, 55/255)
Window.title = "My game clicker"


class MyApp(App, Widget):
    def __init__(self, **kwargs):
        super(MyApp, self).__init__(**kwargs)
        self._keyboard = Window.request_keyboard(self._keyboard_closed, self)
        self._keyboard.bind(on_key_down=self._on_keyboard_down)

        self.balance = 0
        self.level = 0
        self.level_names = ['', 'Noob', 'Beginner', 'Ordinary player', 'Professional', 'Gym Boss']
        self.cost = 0
        self.pay_per_click = 1
        self.level_label = Label(text="Your level:" + str(self.level) + " to level up\nPay per click: 1")
        self.click_btn = Button(text="Click on me\n Your balance: " + str(self.balance))
        self.upgrade_btn = Button(text="Pay me " + str(self.cost))

    def _keyboard_closed(self):
        self._keyboard.unbind(on_key_down=self._on_keyboard_down)
        self._keyboard = None

    def _on_keyboard_down(self, keyboard, keycode, text, modifiers):
        if keycode[1] == 'spacebar':
            self.farm_btn_pressed()
        return True

    def farm_btn_pressed(self, *args):
        self.balance += self.pay_per_click
        self.click_btn.text = "Click on me\n Your balance: " + str(self.balance)
        if not self.balance % 30:
            self.click_btn.text += "\nYou're ultra-mega-good"

    def upgrade_btn_pressed(self, *args):
        if self.balance >= self.cost:
            self.balance -= self.cost
            self.cost += 50
            self.pay_per_click += 1
            self.level += 1
            self.click_btn.text = "Click on me\n Your balance: " + str(self.balance)
            self.upgrade_btn.text = "Pay me " + str(self.cost) + " to level up"
            if self.level <= 5:
                self.level_label.text = "Your level: " + str(self.level) + \
                                        "\nYour rank: " + self.level_names[self.level] + \
                                        "\nPay per click: " + str(self.pay_per_click)
            else:
                self.level_label.text = "Your level: " + str(self.level) + "\nYour rank: " + self.level_names[5] + \
                                        "\nPay per click: " + str(self.pay_per_click)
        else:
            self.upgrade_btn.text = "Pay me " + str(self.cost) + " to level up\nYou're poor, click more!"

    def build(self):
        box = BoxLayout()
        self.click_btn.background_color = (10/255, 0/255, 255/255)
        self.click_btn.bind(on_key_down=self._on_keyboard_down)
        self.upgrade_btn.bind(on_press=self.upgrade_btn_pressed)
        self.upgrade_btn.background_color = (10/255, 200/255, 255/255)
        box.add_widget(self.level_label)
        box.add_widget(self.click_btn)
        box.add_widget(self.upgrade_btn)

        return box


if __name__ == '__main__':
    MyApp().run()

