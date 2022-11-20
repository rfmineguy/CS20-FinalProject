# CS20-FinalProject


Data Structures :
```UML
+ List(): N/A
+ virtual ~List(): N/A
+ virtual At(index: int): T
+ virtual Insert(index: int, elem: T): void = 0
+ virtual Replace(index: int, elem: T): void = 0
+ virtual Remove(index: int): T = 0
+ virtual Length(): int = 0
```

```UML
+ Queue(max_size: int): N/A
+ virtual ~Queue(): N/A
+ Enqueue(item: T): void
+ Dequeue(): T
+ Size(): int
```

```UML
+ Stack(max_size: int): N/A
+ Stack(const Stack&): Stack
+ virtual ~Stack(): N/A
+ virtual Push(elem: T): void = 0
+ virtual Peek(): T = 0
+ virtual Pop(): void = 0
+ virtual Size(): int = 0
```

```UML
+ HashTable(max_slots: int): N/A
+ HashTable(const HashTable&): HashTable ?
+ operator=(const HashTable&): HashTable ?
+ virtual Hash(Key): int = 0
+ virtual Find(key): val = 0
+ virtual Insert(key, val): void = 0
+ virtual Remove(key): void = 0
+ virual Size(): int = 0
```