import random
import time
from typing import Optional, List, Tuple

class Node:
    def __init__(self, key: int, level: int):
        self.key = key

        self.forward = [None] * (level + 1)


class SkipList:
    def __init__(self, max_level: int = 32, p: float = 0.5):

        self.max_level = max_level
        self.p = p
        self.level = 0

        self.header = Node(-1, max_level)
        self.nodes_visited = 0
    
    def reset_counter(self):

        self.nodes_visited = 0
    
    def _random_level(self) -> int:

        level = 0
        while random.random() < self.p and level < self.max_level:
            level += 1
        return level
    
    def insert(self, key: int) -> None:

        update = [None] * (self.max_level + 1)
        current = self.header

        for i in range(self.level, -1, -1):
            while current.forward[i] and current.forward[i].key < key:
                current = current.forward[i]
            update[i] = current
        

        current = current.forward[0]

        if current and current.key == key:
            return
        
        random_level = self._random_level()

        if random_level > self.level:
            for i in range(self.level + 1, random_level + 1):
                update[i] = self.header
            self.level = random_level

        new_node = Node(key, random_level)

        for i in range(random_level + 1):
            new_node.forward[i] = update[i].forward[i]
            update[i].forward[i] = new_node
    
    def search(self, key: int) -> bool:

        self.reset_counter()
        current = self.header
        
        for i in range(self.level, -1, -1):
            while current.forward[i] and current.forward[i].key < key:
                self.nodes_visited += 1
                current = current.forward[i]
            

        current = current.forward[0]
        self.nodes_visited += 1

        return current and current.key == key
    
    def search_count_nodes(self, key: int) -> Tuple[bool, int]:

        self.reset_counter()
        result = self.search(key)
        return result, self.nodes_visited


def test_skip_list_performance(sizes=[5000000, 10000000, 20000000], 
                              searches=1000000):

    results = {}
    
    for size in sizes:
        print(f"Testing Skip List with {size} nodes...")
        
        skip_list = SkipList(max_level=int(size ** 0.5))  
        

        print(f"Generating {size} random keys...")
        keys = random.sample(range(size * 10), size) 
        print(f"Inserting {size} keys into the Skip List...")
        start_time = time.time()
        for key in keys:
            skip_list.insert(key)
        end_time = time.time()
        print(f"Insertion took {end_time - start_time:.2f} seconds")
        

        print(f"Preparing {searches} search keys...")
        existing_keys = random.sample(keys, searches // 2)
        max_key = max(keys)
        non_existing_keys = [max_key + i + 1 for i in range(searches // 2)]
        search_keys = existing_keys + non_existing_keys
        random.shuffle(search_keys)
        
        print(f"Performing {searches} searches...")
        total_nodes_visited = 0
        start_time = time.time()
        
        for key in search_keys:
            _, nodes_visited = skip_list.search_count_nodes(key)
            total_nodes_visited += nodes_visited
            
        end_time = time.time()
        
        avg_nodes_visited = total_nodes_visited / searches
        print(f"Search took {end_time - start_time:.2f} seconds")
        print(f"Average nodes visited: {avg_nodes_visited:.2f}")
        
        results[size] = avg_nodes_visited
    
    return results


if __name__ == "__main__":

    test_sizes = [5000000, 10000000, 20000000] 
    results = test_skip_list_performance(sizes=test_sizes, searches=1000)
    
    print("\nResults:")
    for size, avg_nodes in results.items():
        print(f"Skip List size {size}: Average nodes visited = {avg_nodes:.2f}")
    
