const readline = require('readline');
const fs = require('fs');
const moment = require('moment');
const _ = require('lodash');

const entries = [];

class Entry {
    constructor(type, date, guard = null) {
        this.type = type;
        this.date = moment(date);
        this.guard = guard;
    }
}

class SleepLogEntry {
    constructor(guard) {
        this.guard = guard;
        this.sum = 0;
        this.minutes = new Array(60).fill(0);
    };

    logSleep(start, end) {
        this.sum += end-start;

        for (let i = start; i < end; i++) {
            this.minutes[i]++;
        }
    }

    frequentMinute() {
        let index = 0;

        for (let i = 0; i < 60; i++) {
            if (this.minutes[i] > this.minutes[index]) {
                index = i;
            }
        }

        return index;
    }
}

const sleepLog = {};

const lineReader = readline.createInterface({
    input: fs.createReadStream('input.txt')
});

const fallsAsleepPattern = new RegExp('^\\[(.+)\\]\\sfalls asleep');
const wakesUpPattern = new RegExp('^\\[(.+)\\]\\swakes up');
const shiftBeginsPattern = new RegExp('^\\[(.+)\\]\\sGuard #(\\d+) begins shift');

lineReader.on('line', (line) => {
    var matches = null;

    if (matches = line.match(fallsAsleepPattern)) {
        entries.push(new Entry('SLEEP', matches[1]));
    } else if (matches = line.match(wakesUpPattern)) {
        entries.push(new Entry('WAKE', matches[1]));
    } else if (matches = line.match(shiftBeginsPattern)) {
        entries.push(new Entry('SHIFT', matches[1], matches[2]));
    }
});

lineReader.on('close', () => {
    entries.sort((a, b) => {
        if (a.date > b.date) return 1;
        if (a.date < b.date) return -1;

        return 0;
    });

    let currentGuard;
    let sleepStart;

    for (let entry of entries) {
        if (entry.type == 'SHIFT') {
            currentGuard = entry.guard;
        } else if (entry.type == 'SLEEP') {
            sleepStart = entry.date.minutes();
        } else if (entry.type == 'WAKE') {
            if (!sleepLog[currentGuard]) {
                sleepLog[currentGuard] = new SleepLogEntry(currentGuard);
            }
            sleepLog[currentGuard].logSleep(sleepStart, entry.date.minutes());
        }
    }

    let sleeper = _.maxBy(_.values(sleepLog), n => n.sum);

    console.log('Guard: #' + sleeper.guard + ', slept for: ' + sleeper.sum + ' minutes, and his favorite was: ' + sleeper.frequentMinute() + '. Answer = ' + parseInt(sleeper.guard) * sleeper.frequentMinute());
});
