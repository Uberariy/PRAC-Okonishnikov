import asyncio

async def waiter(e1, e2, e3):
    print('waiting for it ...')
    await e1.wait()
    print('... got it!')
    await e2.wait()
    print('... got 2 it!')
    e3.set()

async def waaiter(e3):
    print('waiting for e3 ...')
    await e3.wait()
    print('... got 3 it!')

async def main():
    # Create an Event object.
    e1, e2, e3 = asyncio.Event(), asyncio.Event(), asyncio.Event()

    # Spawn a Task to wait until 'event' is set.
    waaiter_task = asyncio.create_task(waaiter(e3))
    waiter_task = asyncio.create_task(waiter(e1, e2, e3))

    # Sleep for 1 second and set the event.
    await asyncio.sleep(1)
    e1.set()
    await asyncio.sleep(1)
    e2.set()

    # Wait until the waiter task is finished.
    await asyncio.gather(waaiter_task, waiter_task)

asyncio.run(main())