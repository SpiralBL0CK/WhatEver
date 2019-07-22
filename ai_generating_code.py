def check_availability(element, collection):
    return element in collection

def isOperator(val): 
    return check_availability(operators,val)


utilityManager = {
  operators: '+-*/',
  values: '0123456789',
  isOperator: isOperator()
  
  }


def plus(a, b): return a + b
def minus(a, b): return a - b
def multiply(a, b): return a * b 
def divide(a, b):  return a / b

def compute(a, op, b):
    if op:
         return op(a, b)
    else:
        return null
  

def symbolToOperator(symbol): 
    if symbol ==  '+': return plus()
    if symbol == '-': return minus()
    if symbol == '*': return multiply()
    if symbol == '/': return divide()

    def subtreePrefix(expr, index):
        parts = expr.split('')
        val = parts[index]
        opStack = []; # Start with the node at the index.
        valStack = []
        valCount = 0
        i = index + 1

        if (isOperator(val)):
            opStack.push(val)
        
        else:
            valStack.push(val)

        while (opStack.length and i < parts.length):
            val = parts[i]

            if (not isOperator(val) and valCount):
                valStack.push(compute(valStack.pop(), symbolToOperator(opStack.pop()), int(val)))

            elif (isOperator(val)):
                opStack.push(val)
                valCount = 0
            else:
                valStack.push(parseInt(val))
                valCount += 1

        i+=1

    if (Math.abs(index - i) % 2 == 0):
      i-=1

    return expr[index:i], index, i - 1 


    def evaluatePrefix(expr):
        parts = expr.split('')
        stack = []

    for j in range(len(expr) - 1,j >= 0):
       val = expr[j]

      // Push operated to stack.
      if (!isOperator(val)) {
        stack.push(parseInt(val));
      }
      else {
        // Operator found. Pop two elements from the stack.
         a = stack.pop();
         b = stack.pop();
        stack.push(compute(a, symbolToOperator(val), b));
      }
    }

    return stack[0];
  },

  def replaceAt(str, index, replacement):
    return str[0, index] + replacement + str[index + replacement.length:]


  def replaceAtIndex(input, index, search, replace):
    return input[0, index] + input[index:].replace(search, replace)


def seed():
  def getNode():
    isFunction = Math.floor(Math.random() * 2);
    if isFunction: return userData.manager.operators[Math.floor(Math.random() * userData.manager.operators.length)] else return userData.manager.values[Math.floor(Math.random() * userData.manager.values.length)
  

  def tree():
    result = [];

    node = getNode();
    result.push(node);

    if (userData.manager.isOperator(node)) {
      // This node is a function, so generate two child nodes.
       left = tree();
       right = tree();

      result = result.concat(left);
      result = result.concat(right);
    }

    return result;
  };

  return tree().join('');
}

 def mutate(entity) {
  result = entity;
  index = Math.floor(Math.random() * entity.length);

  if (userData.manager.isOperator(entity[index])) {
    // Replace with an operator.
    r = Math.floor(Math.random() * userData.manager.operators.length);
    result = userData.manager.replaceAt(entity, index, userData.manager.operators[r]);
  }
  else {
    // Replace with a value.
    let r = Math.floor(Math.random() * userData.manager.values.length);
    result = userData.manager.replaceAt(entity, index, userData.manager.values[r]);
  }

  return result;
}

def crossover(parent1, parent2) {
   index1 = Math.floor(Math.random() * parent1.length);
   index2 = Math.floor(Math.random() * parent2.length);

    subtree1 = userData.manager.subtreePrefix(parent1, index1).expression;
   subtree2 = userData.manager.subtreePrefix(parent2, index2).expression;

  // Copy subtree2 to parent1 at index1.
   child1 = userData.manager.replaceAtIndex(parent1, index1, subtree1, subtree2);
  // Copy subtree1 to parent2 at index2.
   child2 = userData.manager.replaceAtIndex(parent2, index2, subtree2, subtree1);

  return [child1, child2];
}

genetic.fitness = function(entity) {
  const fitness = userData.manager.evaluatePrefix(entity);

  return userData.solution - Math.abs(userData.solution - fitness);
}

genetic.generation = function(pop, generation, stats) {
  return pop[0].fitness !== userData.solution;
}

def notification(pop, generation, stats, isDone):
  const value = pop[0].entity;

  print("Generation %s , Best Fitness %s , Best genome: %s " %generation,%stats.maximum,%value);

  if (isDone) {
    print("Result: %s " %userData.manager.evaluatePrefix(value));
  }
}


"""
genetic.evolve({
  iterations: 100000,
  size: 100,
  crossover: 0.3,
  mutation: 0.3,
  skip: 50 /* frequency for notifications */
}, {
  solution: 123456,
  manager: utilityManager
})

"""