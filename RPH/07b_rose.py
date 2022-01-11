from rosemary import Item, update

#--- NORMAL ITEM ---

def test_normal_item_name_doesnt_change():
    item = Item('Bread', days_left=3, quality=5)
    update(item)
    return item.name == 'Bread'

def test_normal_item_decreases_days_left():
    item = Item('Bread', days_left=3, quality=5)
    update(item)
    return item.days_left == 2

def test_normal_item_decreases_quality():
    item = Item('Bread', days_left=3, quality=5)
    update(item)
    return item.quality == 4

def test_normal_item_quality_last_day():
    item = Item('Bread', days_left=1, quality=5)
    update(item)
    return item.quality == 4

def test_normal_item_quality_after_exp_date():
    item = Item('Bread', days_left=0, quality=4)
    update(item)
    return item.quality == 2

def test_normal_item_quality():
    item = Item('Bread', days_left=0, quality=1)
    update(item)
    return 0 <= item.quality <= 50

#--- DIAMOND ---

def test_diamond_name_doesnt_change():
    item = Item('Diamond', days_left=1, quality=100)
    update(item)
    return item.name == 'Diamond'

def test_diamond_decreases_days_left():
    item = Item('Diamond', days_left=1, quality=100)
    update(item)
    return item.days_left == 1

def test_diamond_quality():
    item = Item('Diamond', days_left=1, quality=100)
    update(item)
    return item.quality == 100


#--- CHEESE ---

def test_brie_name_doesnt_change():
    item = Item('Aged Brie', days_left=5, quality=25)
    update(item)
    return item.name == 'Aged Brie'

def test_brie_decreases_quality():
    item = Item('Aged Brie', days_left=5, quality=25)
    update(item)
    return item.quality == 26

def test_brie_quality():
    item = Item('Aged Brie', days_left=5, quality=50)
    update(item)
    return 0 <= item.quality <= 50

def test_brie_quality_after_exp_date():
    item = Item('Aged Brie', days_left=0, quality=25)
    update(item)
    return item.quality == 26

def test_brie_decreases_days_left():
    item = Item('Aged Brie', days_left=5, quality=25)
    update(item)
    return item.days_left == 4

def test_brie_decreases_no_days_left():
    item = Item('Aged Brie', days_left=1, quality=25)
    update(item)
    return item.days_left == 0


#--- TICKETS ---

def test_tickets_name_doesnt_change():
    item = Item('Tickets', days_left=5, quality=25)
    update(item)
    return item.name == 'Tickets'

def test_tickets_decreases_days_left():
    item = Item('Tickets', days_left=5, quality=25)
    update(item)
    return item.days_left == 4

def test_tickets_quality():
    item = Item('Tickets', days_left=12, quality=50)
    update(item)
    return 0 <= item.quality <= 50

def test_tickets_increase_quality_10():
    item = Item('Tickets', days_left=12, quality=25)
    update(item)
    if item.days_left > 10:
        return item.quality == 26

def test_tickets_quality_10():
    item = Item('Tickets', days_left=11, quality=25)
    update(item)
    return item.quality == 26

def test_tickets_increase_quality_6():
    item = Item('Tickets', days_left=8, quality=25)
    update(item)
    if 10 > item.days_left >= 6:
        return item.quality == 27

def test_tickets_quality_6():
    item = Item('Tickets', days_left=6, quality=25)
    update(item)
    return item.quality == 27

def test_tickets_increase_quality_1():
    item = Item('Tickets', days_left=3, quality=25)
    update(item)
    if 6 > item.days_left >= 1:
        return item.quality == 28

def test_tickets_quality_1():
    item = Item('Tickets', days_left=1, quality=25)
    update(item)
    return item.quality == 28

def test_tickets_exp_date():
    item = Item('Tickets', days_left=1, quality=25)
    update(item)
    return item.days_left == 0

def test_tickets_exp_date_quality():
    item = Item('Tickets', days_left=0, quality=3)
    update(item)
    if item.days_left <= 0:
        return item.quality == 0

