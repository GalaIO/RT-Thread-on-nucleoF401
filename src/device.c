/*
 * File      : device.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2007-01-21     Bernard      the first version
 * 2010-05-04     Bernard      add rt_device_init implementation
 * 2012-10-20     Bernard      add device check in register function,
 *                             provided by Rob <rdent@iinet.net.au>
 * 2012-12-25     Bernard      return RT_EOK if the device interface not exist.
 * 2013-07-09     Grissiom     add ref_count support
 *
 *
 *
 *	Author:		GalaIO
 *	Date:			2015-7-21 8:46 PM
 *	Description:
 *			-rt_device_register:	register to rt_object_device_list , rt_object_container.
 *									when you register a device,then will init right now.
 *
 *
 *	Author:		GalaIO
 *	Date:			2015-7-30 4:21 PM
 *	Description:
 *			-info：	add more referance info for user.
 *			
 *
 *
 */

#include <rtthread.h>

#ifdef RT_USING_DEVICE

/**
 * This function registers a device driver with specified name.
 *
 * @param dev the pointer of device driver structure
 * @param name the device driver's name
 * @param flags the flag of device
 *
 * @return the error code, RT_EOK on initialization successfully.
 */
rt_err_t rt_device_register(rt_device_t dev,
                            const char *name,
                            rt_uint16_t flags)
{
    if (dev == RT_NULL)
        return -RT_ERROR;

    if (rt_device_find(name) != RT_NULL)
        return -RT_ERROR;
		//when you register a device,then will init right now.
    rt_object_init(&(dev->parent), RT_Object_Class_Device, name);
    dev->flag = flags;
    dev->ref_count = 0;

    return RT_EOK;
}
RTM_EXPORT(rt_device_register);

/**
 * This function removes a previously registered device driver
 *
 * @param dev the pointer of device driver structure
 *
 * @return the error code, RT_EOK on successfully.
 */
rt_err_t rt_device_unregister(rt_device_t dev)
{
    RT_ASSERT(dev != RT_NULL);

    rt_object_detach(&(dev->parent));

    return RT_EOK;
}
RTM_EXPORT(rt_device_unregister);

/**
 * This function initializes all registered device driver
 *
 * @return the error code, RT_EOK on successfully.
 */
rt_err_t rt_device_init_all(void)
{
    struct rt_device *device;
    struct rt_list_node *node;
    struct rt_object_information *information;
    register rt_err_t result;

    extern struct rt_object_information rt_object_container[];

    information = &rt_object_container[RT_Object_Class_Device];

    /* for each device */
    for (node  = information->object_list.next;
         node != &(information->object_list);
         node  = node->next)
    {
        rt_err_t (*init)(rt_device_t dev);
        device = (struct rt_device *)rt_list_entry(node,
                                                   struct rt_object,
                                                   list);

        /* get device init handler */
        init = device->init;
        if (init != RT_NULL && !(device->flag & RT_DEVICE_FLAG_ACTIVATED))
        {
            result = init(device);
            if (result != RT_EOK)
            {
                rt_kprintf("To initialize device:%s failed. The error code is %d\n",
                           device->parent.name, result);
            }
            else
            {
                device->flag |= RT_DEVICE_FLAG_ACTIVATED;
            }
        }
    }

    return RT_EOK;
}

/**
 * This function finds a device driver by specified name.
 *
 * @param name the device driver's name
 *
 * @return the registered device driver on successful, or RT_NULL on failure.
 *
 *函数：从挂接到对象容器中的设备中，搜索名字为name的设备，搜索到返回设备地址，否则返回RT_NULL。
 *流程：1.进入代码临界区， rt_enter_critical();访问了共享内存，共享资源。
 *      2.获取到RT-Thread的对象容器中，获取设备容器列表，information = &rt_object_container[RT_Object_Class_Device];
 *		  接着得到列表的list句柄，node  = information->object_list.next;利用list的双链表特性，开始遍历list元素。
 *		3.从list句柄中得到rt_object句柄，object = rt_list_entry(node, struct rt_object, list);
 *		  比较name，搜索到设备返回，没有继续直到搜索一圈，返回。
 *
 */
rt_device_t rt_device_find(const char *name)
{
    struct rt_object *object;
    struct rt_list_node *node;
    struct rt_object_information *information;

    extern struct rt_object_information rt_object_container[];

    /* enter critical */
    if (rt_thread_self() != RT_NULL)
        rt_enter_critical();

    /* try to find device object */
    information = &rt_object_container[RT_Object_Class_Device];
	/*把object_list， 下所有node元素，从头到尾，因为object_list是双链表，头尾相连
	 *遍历一遍，并比较“name”，找到就返回，毫不犹豫.
	 *同时 从遍历方式可以看出，容器列表有一个头指针，用来插入删除。
	**/
    for (node  = information->object_list.next;
         node != &(information->object_list);
         node  = node->next)
    {
		/*由struct rt_object的成员 node地址，得到struct rt_object 结构体地址。
		 *第三个参数是 node的偏移的成员名。
        **/
		object = rt_list_entry(node, struct rt_object, list);
        if (rt_strncmp(object->name, name, RT_NAME_MAX) == 0)
        {
            /* leave critical */
            if (rt_thread_self() != RT_NULL)
                rt_exit_critical();

            return (rt_device_t)object;
        }
    }

    /* leave critical */
    if (rt_thread_self() != RT_NULL)
        rt_exit_critical();

    /* not found */
    return RT_NULL;
}
RTM_EXPORT(rt_device_find);

/**
 * This function will initialize the specified device
 *
 * @param dev the pointer of device driver structure
 *
 * @return the result
 *
 *函数：初始化设备对象，并检验RT_DEVICE_FLAG_ACTIVATED和置RT_DEVICE_FLAG_ACTIVATED标志位。
 *流程：1.如果传入参数 dev非空，dev设备的初始化函数非空，同时设备的标志位，没有置已激活状态RT_DEVICE_FLAG_ACTIVATED
 *	      （这说明，在函数注册是应主动把 设备标志flag应异等于为RT_DEVICE_FLAG_DEACTIVATE状态。
 *		2.调用设备的init回调函数，进行初始化操作。如果成功（返回RT_EOK）则返回RT_EOK，失败则打印消息。
 * 
 *		Notes: 对于一个简单设备说，可以不实现dev的open，close，因为他们的工作主要是检查打开参数有效，
 *				检测设备计数是否一致我们可以在这些中加入debug消息，或者在设备被启动时做一些动作等。
 *  			同时如果没有私有数据的初始化等工作，init初始化也可以选择不实现。
 */
rt_err_t rt_device_init(rt_device_t dev)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    /* get device init handler */
    if (dev->init != RT_NULL)
    {
        if (!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
        {
            result = dev->init(dev);
            if (result != RT_EOK)
            {
                rt_kprintf("To initialize device:%s failed. The error code is %d\n",
                           dev->parent.name, result);
            }
            else
            {
                dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
            }
        }
    }

    return result;
}

/**
 * This function will open a device
 *
 * @param dev the pointer of device driver structure
 * @param oflag the flags for device open, oflag only inputed with RT_DEVICE_OFLAG_RDONLY or RT_DEVICE_OFLAG_WRONLY or RT_DEVICE_OFLAG_RDWR
 *
 * @return the result
 *
 *函数：打开设备文件，并检验RT_DEVICE_FLAG_ACTIVATED,RT_DEVICE_FLAG_STANDALONE和置RT_DEVICE_OFLAG_OPEN标志位，最后自加设备的计数。
 *流程：1.检查设备文件参数是否为空。
 *		2.判断oflag的属性是否是可读，可写，或者读写，并且不能使别的值，同时比较dev的open_flag是否支持这种 属性。全成立则继续执行，否则返回错误。
 *		3.如果设备没有被激活，同时设备初始化回调函数非空，即初始化设备，并置RT_DEVICE_FLAG_ACTIVATED标志，初始化失败返回，并打印消息。
 *		4.如果该设备是独占设备，且已经被打开，那么返回设备忙。
 *		5.调用设备open回调函数执行，对返回值检查，open成功后（返回RT_EOK），则置标志量RT_DEVICE_OFLAG_OPEN，同时自加设备的计数dev->ref_count++;返回。
 *
 *		Notes： a.如果该设备具有RT_DEVICE_FLAG_STANDALONE独占属性，那么rt_device_open会检查设备RT_DEVICE_OFLAG_OPEN标志，来保证
 *				设备不会被重复打开，如果设备不是独占，那么会背重复打开，同时自加设备计数值，为设备close函数作参考。
 *				b.在rt-thread的1.2版本中，不推荐使用rt_device_init_all函数的原因，就在 rt_device_open中，会自动检查设备的打开状态，自行调用初始化设备回调函数。
 *        		c.设备在使用是 只允许修改open_flag，不能修改flag，这是定律。
 *				d.对于一个简单设备说，可以不实现dev的open，close，因为他们的工作主要是检查打开参数有效，
 *				检测设备计数是否一致我们可以在这些中加入debug消息，或者在设备被启动时做一些动作等。
 *  			同时如果没有私有数据的初始化等工作，init初始化也可以选择不实现。
**/
rt_err_t rt_device_open(rt_device_t dev, rt_uint16_t oflag)
{
    rt_err_t result = RT_EOK;
	
    RT_ASSERT(dev != RT_NULL);
	
	/*
	 *@added check if oflag == RT_DEVICE_OFLAG_RDONLY or RT_DEVICE_OFLAG_WRONLY or RT_DEVICE_OFLAG_RDWR
	 *		 check if dev->flag has oflag attributes.
	**/
	if(!((RT_DEVICE_OFLAG_RDONLY == oflag || 
		  RT_DEVICE_OFLAG_WRONLY == oflag || 
		  RT_DEVICE_OFLAG_RDWR   == oflag) &&  
		 (dev->flag & oflag))){
		//wrong param ==> oflag or wrong attri ==> dev->flag	 
		RT_DEBUG_LOG(RT_DEBUG_DEVICE,("wrong param ==> oflag or wrong attri ==> dev->flag!!\r\n"));
		return RT_ERROR;
	}
    /* if device is not initialized, initialize it. */
    if (!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
        if (dev->init != RT_NULL)
        {
            result = dev->init(dev);
            if (result != RT_EOK)
            {
                rt_kprintf("To initialize device:%s failed. The error code is %d\n",
                           dev->parent.name, result);

                return result;
            }
        }

        dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
    }

    /* device is a stand alone device and opened */
    if ((dev->flag & RT_DEVICE_FLAG_STANDALONE) &&
        (dev->open_flag & RT_DEVICE_OFLAG_OPEN))
    {
		RT_DEBUG_LOG(RT_DEBUG_DEVICE,("sorry this is a standalong device, cannot shared it, please waiting!!\r\n"));
        return -RT_EBUSY;
    }

    /* call device open interface */
    if (dev->open != RT_NULL)
    {
        result = dev->open(dev, oflag);
		/*@added modify oflag autoly.*/
		dev->open_flag |= oflag;
    }

    /* set open flag */
    if (result == RT_EOK || result == -RT_ENOSYS)
    {
        dev->open_flag = oflag | RT_DEVICE_OFLAG_OPEN;

        dev->ref_count++;
        /* don't let bad things happen silently. If you are bitten by this assert,
         * please set the ref_count to a bigger type. */
        RT_ASSERT(dev->ref_count != 0);
    }

    return result;
}
RTM_EXPORT(rt_device_open);

/**
 * This function will close a device
 *
 * @param dev the pointer of device driver structure
 *
 * @return the result
 *
 *函数：关闭设备文件，检查设备计数器，并设置dev的open flag。
 *流程：1.判断设备dev句柄是否为空。
 *		2.检查设备的计数器是否合理，是否出现打开和关闭计数，不一致的情况，出现即报错。
 *		3.若设备的关闭回调函数非空，则调用。
 *		4.检查回调函数返回结果，默认是RT_EOK，并设置open flag为RT_DEVICE_OFLAG_CLOSE；
 *
 *		Notes: 对于一个简单设备说，可以不实现dev的open，close，因为他们的工作主要是检查打开参数有效，
 *				检测设备计数是否一致我们可以在这些中加入debug消息，或者在设备被启动时做一些动作等。
 *  			同时如果没有私有数据的初始化等工作，init初始化也可以选择不实现。
 */
rt_err_t rt_device_close(rt_device_t dev)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    if (dev->ref_count == 0){
        RT_DEBUG_LOG(RT_DEBUG_DEVICE,("the time of device_open and device_close is not match!!\r\n"));
		return -RT_ERROR;
	}

    dev->ref_count--;

    if (dev->ref_count != 0)
        return RT_EOK;

    /* the device closed when dev->ref_count = 0, call device close interface */
    if (dev->close != RT_NULL)
    {
        result = dev->close(dev);
    }

    /* set open flag */
    if (result == RT_EOK || result == -RT_ENOSYS)
        dev->open_flag = RT_DEVICE_OFLAG_CLOSE;

    return result;
}
RTM_EXPORT(rt_device_close);

/**
 * This function will read some data from a device.
 *
 * @param dev the pointer of device driver structure
 * @param pos the position of reading
 * @param buffer the data buffer to save read data
 * @param size the size of buffer
 *
 * @return the actually read size on successful, otherwise negative returned.
 *
 * @note since 0.4.0, the unit of size/pos is a block for block device.
 *
 *函数：从设备中在偏移为pos，读出size大小的buffer数据块，并检查打开参数。
 *流程：1.检查设备dev句柄是否为空。
 *		2.检查设备是否被打开，检查open_flag参数是否可读。
 *		3.调用回调函数，返回结果。
 *
 *		Notes:对于一个设备的 read回调函数，应该考虑如何去处理buffer和pos+size，
 *			  其他逻辑rt_device_read基本以处理好，框架已经基本建立，主要是如何读数据。
 *
 */
rt_size_t rt_device_read(rt_device_t dev,
                         rt_off_t    pos,
                         void       *buffer,
                         rt_size_t   size)
{
    RT_ASSERT(dev != RT_NULL);

	/*@added the device is opened and readable.*/
	if(!((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
	    ((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
	    (dev->open_flag & RT_DEVICE_OFLAG_RDONLY)))){
		RT_DEBUG_LOG(RT_DEBUG_DEVICE,("in read:the device is not opened correctly check oflag or open it again!!\r\n"));
		return RT_ERROR;
	}
	
    if (dev->ref_count == 0)
    {
        rt_set_errno(-RT_ERROR);
        return 0;
    }

    /* call device read interface */
    if (dev->read != RT_NULL)
    {
        return dev->read(dev, pos, buffer, size);
    }

    /* set error code */
    rt_set_errno(-RT_ENOSYS);

    return 0;
}
RTM_EXPORT(rt_device_read);

/**
 * This function will write some data to a device.
 *
 * @param dev the pointer of device driver structure
 * @param pos the position of written
 * @param buffer the data buffer to be written to device
 * @param size the size of buffer
 *
 * @return the actually written size on successful, otherwise negative returned.
 *
 * @note since 0.4.0, the unit of size/pos is a block for block device.
 *
 *函数：向设备写入偏移为pos，大小为size的buffer数据块，并检查打开标志。
 *流程：1.检查设备dev句柄是否为空。
 *		2.检查设备是否打开，是否可写。
 *		3.直接调用设备的 write回调函数。
 *
 *		Notes：对于编写write回调函数来说，只用关注如何协数据块即可，基本框架已建立。
 */
rt_size_t rt_device_write(rt_device_t dev,
                          rt_off_t    pos,
                          const void *buffer,
                          rt_size_t   size)
{
    RT_ASSERT(dev != RT_NULL);

	/*@added the device is opened and readable.*/
	if(!((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
	    ((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
		(dev->open_flag & RT_DEVICE_OFLAG_WRONLY)))){
		RT_DEBUG_LOG(RT_DEBUG_DEVICE,("in write:the device is not opened correctly check oflag or open it again!!\r\n"));
		return RT_ERROR;
	}
	
    if (dev->ref_count == 0)
    {
        rt_set_errno(-RT_ERROR);
        return 0;
    }

    /* call device write interface */
    if (dev->write != RT_NULL)
    {
        return dev->write(dev, pos, buffer, size);
    }

    /* set error code */
    rt_set_errno(-RT_ENOSYS);

    return 0;
}
RTM_EXPORT(rt_device_write);

/**
 * This function will perform a variety of control functions on devices.
 *
 * @param dev the pointer of device driver structure
 * @param cmd the command sent to device
 * @param arg the argument of command
 *
 * @return the result
 *
 *函数：向设备发送命令和参数，让设备进行反应。
 *
 */
rt_err_t rt_device_control(rt_device_t dev, rt_uint8_t cmd, void *arg)
{
    RT_ASSERT(dev != RT_NULL);

    /* call device write interface */
    if (dev->control != RT_NULL)
    {
        return dev->control(dev, cmd, arg);
    }

    return RT_EOK;
}
RTM_EXPORT(rt_device_control);

/**
 * This function will set the indication callback function when device receives
 * data.
 *
 * @param dev the pointer of device driver structure
 * @param rx_ind the indication callback function
 *
 * @return RT_EOK
 *
 *函数：向设备添加rx_ind属性即可，让用户选择在数据到达时调用。
 *
 */
rt_err_t
rt_device_set_rx_indicate(rt_device_t dev,
                          rt_err_t (*rx_ind)(rt_device_t dev, rt_size_t size))
{
    RT_ASSERT(dev != RT_NULL);

    dev->rx_indicate = rx_ind;

    return RT_EOK;
}
RTM_EXPORT(rt_device_set_rx_indicate);

/**
 * This function will set the indication callback function when device has
 * written data to physical hardware.
 *
 * @param dev the pointer of device driver structure
 * @param tx_done the indication callback function
 *
 * @return RT_EOK
 *
 *函数：向设备添加tx_done属性即可，让用户选择在数据发送完成时调用。
 *
 */
rt_err_t
rt_device_set_tx_complete(rt_device_t dev,
                          rt_err_t (*tx_done)(rt_device_t dev, void *buffer))
{
    RT_ASSERT(dev != RT_NULL);

    dev->tx_complete = tx_done;

    return RT_EOK;
}
RTM_EXPORT(rt_device_set_tx_complete);

#endif
