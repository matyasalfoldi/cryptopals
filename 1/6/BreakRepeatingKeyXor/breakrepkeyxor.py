import base64

FreqTable = {
    101:12.702,
    116:9.056,
    97:8.167,
    111:7.507,
    105:6.966,
    110:6.749,
    32:13,
    115:6.327,
    104:6.094,
    114:5.987,
    100:4.253,
    108:4.025,
    117:2.758,
    99:2.782,
    109:2.406,
    119:2.360,
    102:2.228,
    103:2.015,
    121:1.974,
    112:1.929,
    98:1.492,
    118:0.978,
    120:0.150,
    107:0.772,
    69:12.702,
    84:9.056,
    65:8.167,
    79:7.507,
    73:6.966,
    78:6.749,
    83:6.327,
    72:6.094,
    82:5.987,
    68:4.253,
    76:4.025,
    85:2.758,
    67:2.782,
    87:2.360,
    77:2.406,
    70:2.228,
    89:1.974,
    80:1.929,
    66:1.492,
    71:2.015,
    86:0.978,
    88:0.150,
    75:0.772
    }

UsefulChars = [9, 10, 13]

def calc_value(xored):
    score = 0
    for i in range(len(xored)):
        if FreqTable.get(xored[i]):
            score += FreqTable.get(xored[i])
        elif xored[i] > 126 or (xored[i] < 32 and (xored[i] not in UsefulChars)):
            score = -999999
    return score


def best_value(inputs):
    scores = {}
    for key in range(256):
        scores[key] = calc_value([input ^ key for input in inputs])
    return sorted(scores.items(), key=lambda x: x[1], reverse=True)[0][0]


def hamming_dist(first, second):
    return sum([bin(byte).count('1') for byte in bytes([f ^ s for f, s in zip(first,second)])])


def get_key_len(hexValues):
    keyLenScores = {}
    for keyLen in range(2,41):
        dist = 0.0
        c = 0
        for i in range(0,len(hexValues),2*keyLen):
            first  = hexValues[i:i+keyLen]
            second = hexValues[i+keyLen:i+(2*keyLen)]
            dist += hamming_dist(first, second)/keyLen
            c += 1
        keyLenScores[keyLen] = dist / c
    minValue = sorted(keyLenScores.items(), key=lambda x: x[1])[0][1]
    return [key for key,value in keyLenScores.items() if value == minValue]
            

def create_blocks(hexValues, blockSize):
    blocks = len(hexValues) / blockSize if len(hexValues) % blockSize == 0 else (len(hexValues) / blockSize) + 1
    hexBlocks = []
    for i in range(0, len(hexValues), blockSize):
        end = i + blockSize if (i + blockSize) < len(hexValues) else len(hexValues)
        hexBlocks.append(hexValues[i:end])
    return hexBlocks


def transpose_blocks(hexBlocks, keyLen):
    transHexBlocks = [[] for _ in range(keyLen)]
    for hexBlock in hexBlocks:
        for i in range(len(hexBlock)):
            transHexBlocks[i].append(hexBlock[i])
    return transHexBlocks


def rep_key_xor(hexValues, key):
    hexBytes = [hexValue for hexValue in hexValues]
    print(''.join([chr(hexBytes[i] ^ key[i % len(key)]) for i in range(len(hexBytes))]))


def main():
    with open('input.txt') as file:
        hexValues = base64.b64decode(file.read())
    keyLens = get_key_len(hexValues)
    for keyLen in keyLens:
        hexBlocks = create_blocks(hexValues, keyLen)
        transHexBlocks = transpose_blocks(hexBlocks, keyLen)
        key = [best_value(transHexBlock) for transHexBlock in transHexBlocks]
        rep_key_xor(hexValues, key)


if __name__ == "__main__":
    main()
